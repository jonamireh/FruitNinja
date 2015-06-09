#include "World.h"
#include "DeferredShader.h"
#include "SimpleTextureShader.h"
#include "GuardEntity.h"
#include "ArcheryCamera.h"

using namespace glm;
using namespace std;



DeferredShader::DeferredShader(std::string vertShader, std::string fragShader, Skybox* skybox)
	: Shader(vertShader, fragShader), skybox(skybox), gbuffer(), skyShader("simpleVert.glsl", "simpleFrag.glsl"),
	renderer("lightVert.glsl", "pointLightFrag.glsl", &gbuffer, &dirShadowMapBuffer), disp_mode(deferred),
	fireShader("FireVert.glsl", "FireGeom.glsl", "FireFrag.glsl"), arcShader("arcVertex.glsl", "arcFrag.glsl"), shadowBufferShader(&dirShadowMapBuffer)
{
	emitters.push_back(new FlameEmitter);
	//emitters.push_back(new FireEmitter);
	gbuffer.Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	dirShadowMapBuffer.init(SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glBindAttribLocation(getProgramID(), 1, "aNormal");

	uViewMatrixHandle = getUniformHandle("uViewMatrix");
	uModelMatrixHandle = getUniformHandle("uModelMatrix");
	uProjMatrixHandle = getUniformHandle("uProjMatrix");
	UtexHandle = getUniformHandle("Utex");
	UflagHandle = getUniformHandle("Uflag");
	uBoneFlagHandle = getUniformHandle("uBoneFlag");
	uBonesHandle = getUniformHandle("uBones[0]");
	UdColorHandle = getUniformHandle("UdColor");
}

void DeferredShader::geomPass(mat4& view_mat, std::vector<GameEntity*> ents)
{
	map<MeshSet*, vector<GameEntity*>> objectMeshes;

	for (int i = 0; i < ents.size(); i++) {
		map<MeshSet*, vector<GameEntity*>>::iterator meshItr = objectMeshes.find(ents[i]->mesh);
		objectMeshes[ents[i]->mesh].push_back(ents[i]);
	}

	check_gl_error("Before geom pass");

	gbuffer.BindForGeomPass();

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUniformMatrix4fv(uViewMatrixHandle, 1, GL_FALSE, value_ptr(view_mat));
	glUniformMatrix4fv(uProjMatrixHandle, 1, GL_FALSE, value_ptr(projection));

	for (auto& currMeshSet: objectMeshes) {
		
		std::vector<Mesh*> meshes = currMeshSet.first->getMeshes();

		for (int j = 0; j < meshes.size(); j++)
		{
			Mesh* mesh = meshes[j];
			glBindVertexArray(mesh->VAO);

			if (mesh->textures.size() > 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textures.at(0).id);
				glUniform1i(UtexHandle, 0);
				glUniform1i(UflagHandle, 1);
			}
			else {

				glUniform1i(UflagHandle, 0);
			}
			
			if (mesh->bones.size() > 0)
			{
				glUniform1i(uBoneFlagHandle, 1);
			}
			else
			{
				glUniform1i(uBoneFlagHandle, 0);
			}

			Material material = mesh->mat;
			glUniform3fv(UdColorHandle, 1, value_ptr(material.diffuse));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

			for (auto& entity: currMeshSet.second) {
				glUniformMatrix4fv(uModelMatrixHandle, 1, GL_FALSE, value_ptr(entity->getModelMat()));

				if (mesh->bones.size() > 0)
				{
					glUniformMatrix4fv(uBonesHandle, entity->boneTransformations[j].size(), GL_FALSE, value_ptr(entity->boneTransformations[j][0]));
				}

				//check_gl_error("Mesh.draw before texture");

				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

				//check_gl_error("Mesh.draw after texture");

			}
			if (mesh->textures.size() > 0) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			
		}
	}
	glDepthMask(GL_FALSE);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}



void DeferredShader::draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights)
{
    std::vector<GameEntity*> entsInView = get_objects_in_view(ents, camera->getViewMatrix());
	gbuffer.StartFrame();
	geomPass(camera->getViewMatrix(), entsInView);

	if (disp_mode == four_screen) {
		lightPass();
	}
	else {
		renderer.draw(camera, ents, lights);
		
		skyboxPass(camera);
		particlePass(camera, lights);
		archeryArcPass(camera);
		finalPass();
		//shadowBufferShader.draw();
	}
		
	if (keys[GLFW_KEY_4])
		disp_mode = deferred;
	if (keys[GLFW_KEY_5])
		disp_mode = four_screen;
}

void DeferredShader::particlePass(Camera* camera, std::vector<Light*> lights) {
	glUseProgram(fireShader.getProgramID());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	fireShader.draw(camera, emitters, lights);
}

void DeferredShader::archeryArcPass(Camera* camera) {
	ArcheryCamera* a_test = dynamic_cast<ArcheryCamera*>(camera);
	if (a_test != nullptr)
	{
		glUseProgram(arcShader.getProgramID());

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		arcShader.draw(a_test);
		glDisable(GL_BLEND);
	}
}

void DeferredShader::skyboxPass(Camera* camera)
{
	glUseProgram(skyShader.getProgramID());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	skyShader.draw(camera->getViewMatrix(), skybox);
}

void DeferredShader::finalPass()
{
	getProgramID();
	gbuffer.BindForFinalPass();
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void DeferredShader::startLightPasses()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	gbuffer.BindAllForReading();
	glClear(GL_COLOR_BUFFER_BIT);
}


void DeferredShader::draw(glm::mat4& view_mat, GameEntity* entity)
{
	std::cout << "Fuck you\n";
}

void DeferredShader::lightPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gbuffer.BindForReading();

	GLsizei HalfWidth = (GLsizei)(SCREEN_WIDTH / 2.0f);
	GLsizei HalfHeight = (GLsizei)(SCREEN_HEIGHT / 2.0f);

	gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, HalfHeight, HalfWidth, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		HalfWidth, HalfHeight, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	/*gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		HalfWidth, 0, SCREEN_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/
}

DeferredShader::~DeferredShader() {
	for (int i = 0; i < emitters.size(); i++) {
		delete emitters[i];
	}
}