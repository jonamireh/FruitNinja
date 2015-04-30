#include "Shader.h"

using namespace std;
using namespace GLSL;

Shader::Shader()
{
}

Shader::Shader(string vertShader, string fragShader)
{
    GLint rc;

    // Create shader handles
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shader sources
    const char *vshader = textFileRead(vertShader.c_str());
    const char *fshader = textFileRead(fragShader.c_str());
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glShaderSource(fragmentShader, 1, &fshader, NULL);

    // Compile vertex shader
    glCompileShader(vertexShader);
    printError();
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &rc);
    printShaderInfoLog(vertexShader);
    if (!rc)
    {
        printf("Error compiling vertex shader %s\n", vertShader.c_str());
    }

    // Compile fragment shader
    glCompileShader(fragmentShader);
    printError();
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &rc);
    printShaderInfoLog(fragmentShader);
    if (!rc)
    {
        printf("Error compiling fragment shader %s\n", fragShader.c_str());
    }

    // Create the program and link
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    printError();
    glGetProgramiv(program, GL_LINK_STATUS, &rc);
    printProgramInfoLog(program);
    if (!rc)
    {
        printf("Error linking shaders %s and %s\n", vertShader.c_str(), fragShader.c_str());
    }

    setupHandles();

    assert(printError() == GL_NO_ERROR);
}

void Shader::setupHandles()
{
    int total = -1;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &total);
    for (int i = 0; i<total; ++i)
    {
        int name_len = -1, num = -1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveUniform(program, GLuint(i), sizeof(name) - 1,
            &name_len, &num, &type, name);
        name[name_len] = 0;
        GLuint location = glGetUniformLocation(program, name);
        uniforms.insert(pair<string, GLint>(name, location));
    }

    total = -1;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &total);
    for (int i = 0; i<total; ++i)
    {
        int name_len = -1, num = -1;
        GLenum type = GL_ZERO;
        char name[100];
        glGetActiveAttrib(program, GLuint(i), sizeof(name) - 1,
            &name_len, &num, &type, name);
        name[name_len] = 0;
        GLuint location = glGetAttribLocation(program, name);
        attributes.insert(pair<string, GLint>(name, location));
    }
}

GLint Shader::getAttributeHandle(string name)
{
    GLint handle;
    try
    {
        handle = attributes.at(name);
    }
    catch (exception& e)
    {
        cout << "The attribute name '" << name << "' is not bound to this shader." << endl;
        assert(false);
    }
    return handle;
}

GLint Shader::getUniformHandle(string name)
{
    GLint handle;
    try
    {
        handle = uniforms.at(name);
    }
    catch (exception& e)
    {
        cout << "The uniform name '" << name << "' is not bound to this shader." << endl;
        assert(false);
    }
    return handle;
}

GLuint Shader::getProgramID()
{
    return program;
}


bool Shader::check_gl_error(std::string msg) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << msg << ": OpenGL Error: " << error << std::endl;
		return true;
	}
	return false;
}