#ifndef MODEL_H
#define MODEL_H
#include "ShaderHelp.h"
#include <glm/glm.hpp>

class Sector;

class Model
{
    public:
        Model();
        ~Model();

        void setVertexData(GLfloat * f,int size);
        void setNormalData(GLfloat * f);//size is vertex size
        void setColorData(GLfloat * f,int size);
        void setElementData(GLushort * f,int size);
        void setMode(GLenum m);
        void genVAO();
        void render();
        void splitModel(Model** o1, Model** o2);
        int getVertexSize(){return vsize;}
        glm::vec3 getColor(){return (color?((glm::vec3*)color)[0]:glm::vec3());}

    protected:

    int vsize;
    int csize;//should be the same as vsize...
    int esize;
    GLfloat * vertex;
    GLfloat * color;
    GLfloat * norm;
    GLushort * elements;

    GLuint vao;
    GLuint vbo[3];
    GLuint vbn;
    GLenum mode;

    friend class OptiModel;
};

Model* loadModel(std::string s);
Model* genCube2(glm::vec3 s, glm::vec3 c);

#endif // MODEL_H
