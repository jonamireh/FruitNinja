//
// Shinjiro Sueda
// October, 2014
//
// Adapted from:
//   GLSL_helper.h
//   CSC473
//
//    Many useful helper functions for GLSL shaders - gleaned from various sources including orange book
//    Created by zwood on 2/21/10.

#include <GL/glew.h>

#pragma once
#ifndef __GLSL__
#define __GLSL__

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

namespace GLSL {
	
	int printError();
	void printProgramInfoLog(GLuint program);
	void printShaderInfoLog(GLuint shader);
	GLint getUniLoc(GLuint program, const GLchar *name);
	void checkVersion();
	int textFileWrite(const char *filename, char *s);
	char *textFileRead(const char *filename);
	GLint getAttribLocation(const GLuint program, const char varname[]);
	GLint getUniformLocation(const GLuint program, const char varname[]);
	void enableVertexAttribArray(const GLint handle);
	void disableVertexAttribArray(const GLint handle);
	void vertexAttribPointer(const GLint handle, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
}

#endif
