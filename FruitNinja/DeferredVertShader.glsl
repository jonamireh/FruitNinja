#version 150

in vec3 aPosition;
in vec2 aTextCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

out vec2 vTextCoord;

void main( void )
{
	
	gl_Position		= uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	vTextCoord = aTextCoord;
	//gl_TexCoord[0] = aTextCoord;
}
