#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include "Macros.h"

#define sign(s) (s>0?1:-1)
//#define for_each(x , y , z)  for(x::iterator y = z.begin(); y != z.end(); y++)

using namespace std;

Model::Model() {
    vertex = NULL;
    color = NULL;
    elements = NULL;
    norm = NULL;
    mode = GL_TRIANGLES;
    vao=0;
    vbn=0;
}

Model::~Model() {
    if(vertex)
        delete[] vertex;
    if(color)
        delete[] color;
    if(elements)
        delete[] elements;
    if(norm)
        delete[] norm;

    if(vao) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&vao);
        glDeleteBuffers(3,vbo);
    }
    if(vbn)
        glDeleteBuffers(1,&vbn);
}

void Model::setVertexData(GLfloat * f,int size) {
    vertex=f;
    vsize=size;
}

void Model::setNormalData(GLfloat * f) {
    if(norm)
        delete[] norm;

    norm=f;
}

void Model::setColorData(GLfloat * f,int size) {
    color=f;
    csize=size;
}

void Model::setElementData(GLushort * f,int size) {
    elements=f;
    esize=size;
}

void Model::setMode(GLenum m) {
    mode = m;
}

void Model::genVAO() {
    bool first=!vao;
    if(first)
        glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    if(first)
        glGenBuffers(3,vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vsize,vertex,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*csize,color,GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLshort)*esize,elements,GL_STATIC_DRAW);

    if(norm){
        if(!vbn)
            glGenBuffers(1,&vbn);
        glBindBuffer(GL_ARRAY_BUFFER, vbn);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vsize,norm,GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(3);
    }

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Model::render() {
    if(!vao)
        genVAO();

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
    if(elements)
        glDrawElements(mode,esize,GL_UNSIGNED_SHORT,NULL);
    else
        glDrawArrays(mode,0,vsize/3);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

Model* loadModel(string s) {

    ifstream in(s.c_str());

    int vsize,fsize;
    string tmp;
    getline(in,tmp);

    if(tmp.find("ply") !=0 )
        return NULL;

    while(true) {
        getline(in,tmp);
        if(tmp.find("comment") == 0)
            continue;

        if(tmp.find("end_header") == 0)
            break;

        if(tmp.find("element") == 0) {
            if(tmp.find("vertex") != string::npos)
                vsize = atoi(tmp.substr(tmp.find("vertex")+6).c_str());
            if(tmp.find("face") != string::npos)
                fsize = atoi(tmp.substr(tmp.find("face")+4).c_str());

        }


    }

    GLfloat * vert = new GLfloat[vsize*3];
    GLfloat * color = new GLfloat[vsize*3];
    GLfloat discard;

    for(int x=0; x<vsize; x++) {

        in >> vert[x*3];
        in >> vert[x*3+1];
        in >> vert[x*3+2];

        in >> discard;//discard normals for now
        in >> discard;
        in >> discard;

        in >> color[x*3];
        in >> color[x*3+1];
        in >> color[x*3+2];

        for(int y=0; y<3; y++)
            color[x*3+y]= color[x*3+y]/255.0;

    }

    list<GLushort> stuff;

    GLushort t,t2;
    GLushort q1,q2,q3,q4;

    for(int x=0; x<fsize; x++) {

        in >> t;

        if(t == 3) {
            in>>t2;
            stuff.push_back(t2);
            in>>t2;
            stuff.push_back(t2);
            in>>t2;
            stuff.push_back(t2);
        }

        if(t == 4) {
            in>>q1;
            in>>q2;
            in>>q3;
            in>>q4;

            stuff.push_back(q1);
            stuff.push_back(q2);
            stuff.push_back(q3);
            stuff.push_back(q1);
            stuff.push_back(q3);
            stuff.push_back(q4);
        }


    }

    GLushort * elm = new GLushort[stuff.size()];
    int x=0;

    for_each(list<GLushort> , it , stuff) {
        elm[x]=*it;
        x++;
    }

    Model* m = new Model();

    m->setVertexData(vert,vsize*3);
    m->setColorData(color,vsize*3);
    m->setElementData(elm,stuff.size());
    m->genVAO();

    return m;
}

Model* genCube2(glm::vec3 s, glm::vec3 c){

    GLfloat *vert = new GLfloat[108];
    GLfloat *norm = new GLfloat[108];
    GLfloat *color = new GLfloat[108];
    GLushort *elm = new GLushort[36];

    GLfloat tmp[]= {0,0,0, s.x,0,0, s.x,s.y,0,
                    0,0,0, s.x,s.y,0, 0,s.y,0,
                    0,s.y,s.z, s.x,s.y,s.z, s.x,0,s.z,
                    0,s.y,s.z, s.x,0,s.z, 0,0,s.z,
                    s.x,0,s.z, s.x,s.y,s.z, s.x,s.y,0,
                    s.x,0,s.z, s.x,s.y,0, s.x,0,0,
                    0,0,0, 0,s.y,0, 0,s.y,s.z,
                    0,0,0, 0,s.y,s.z, 0,0,s.z,
                    0,0,s.z, s.x,0,s.z, s.x,0,0,
                    0,0,s.z, s.x,0,0, 0,0,0,
                    0,s.y,0, s.x,s.y,0, s.x,s.y,s.z,
                    0,s.y,0, s.x,s.y,s.z, 0,s.y,s.z,
                   };

    GLfloat tmp2[]= {0,0,-1, 0,0,-1, 0,0,-1,
                    0,0,-1, 0,0,-1, 0,0,-1,
                    0,0,1, 0,0,1, 0,0,1,
                    0,0,1, 0,0,1, 0,0,1,
                    1,0,0, 1,0,0, 1,0,0,
                    1,0,0, 1,0,0, 1,0,0,
                    -1,0,0, -1,0,0, -1,0,0,
                    -1,0,0, -1,0,0, -1,0,0,
                    0,-1,0, 0,-1,0, 0,-1,0,
                    0,-1,0, 0,-1,0, 0,-1,0,
                    0,1,0, 0,1,0, 0,1,0,
                    0,1,0, 0,1,0, 0,1,0,
                   };

    /*GLfloat uv[]= {c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                   c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                   c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                   c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                   c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                   c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                  };*/

    /*GLushort el[]= {0,1,2,3,4,5,6,7,
                    8,9,10,11,12,13,14,15,
                    16,17,18,19,20,21,22,23,
                  };*/

    for(int x=0; x<108; x++) {
        vert[x]=tmp[x];
        norm[x]=tmp2[x];
        //color[x]=uv[x];
    }
    for(int x=0;x<36;x++){
        ((glm::vec3*)color)[x]=c;
    }

    for(int x=0;x<36;x++)
        elm[x]=x;

    Model* m = new Model();

    m->setVertexData(vert,108);
    m->setNormalData(norm);
    m->setColorData(color,108);
    m->setElementData(elm,36);
    m->setMode(GL_TRIANGLES);
    //m->genVAO();

    return m;
}

Model* genCube(glm::vec3 s, glm::vec3 c){

    GLfloat *vert = new GLfloat[24];
    //GLfloat *norm = new GLfloat[24];
    GLfloat *color = new GLfloat[24];
    GLushort *elm = new GLushort[24];

    GLfloat tmp[]= {0,0,0, s.x,0,0, s.x,s.y,0, 0,s.y,0,
                    0,0,s.z, s.x,0,s.z, s.x,s.y,s.z, 0,s.y,s.z,
                   };

    /*GLfloat tmp2[]= {-1,-1,-1, 1,-1,-1, 1,1,-1, -1,1,-1,
                    -1,-1,1, 1,-1,1, 1,1,1, -1,1,1,
                   };*/

    GLfloat uv[]= {c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                   c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z, c.x,c.y,c.z,
                  };

    GLushort el[]= {0,1,2,3,7,6,5,4,
                    5,6,2,1,0,3,7,4,
                    4,5,1,0,3,2,6,7,
                  };

    for(int x=0; x<24; x++) {
        vert[x]=tmp[x];
        //norm[x]=tmp2[x];
        color[x]=uv[x];
        elm[x]=el[x];
    }

    Model* m = new Model();

    m->setVertexData(vert,24);
    //m->setNormalData(norm);
    m->setColorData(color,24);
    m->setElementData(elm,24);
    m->setMode(GL_QUADS);
    //m->genVAO();

    return m;
}



