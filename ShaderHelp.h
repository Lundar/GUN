#ifndef SHADERHELP_H_INCLUDED
#define SHADERHELP_H_INCLUDED
#include <dirent.h>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <GL/glew.h>

extern std::vector<GLuint> subShaders;
extern std::map<std::string,GLuint> shaders;

void printLog(GLuint obj);

GLuint loadShader(std::string s,GLenum type);

#ifndef __scanDirectory
#define __scanDirectory
    void scanDirectory(std::vector<std::string> &files, std::string dirr);
#endif

void setupShaders();

GLuint getShader(std::string s);

void setupVAO(GLfloat *texCoords, int texSize, GLfloat *vertexCoords, int verSize, GLuint &vao, GLuint (&vbo)[2]);

void cleanupShaders();

#endif // SHADERHELP_H_INCLUDED
