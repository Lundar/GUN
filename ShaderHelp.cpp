#include "ShaderHelp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>


using namespace std;

vector<GLuint> subShaders;
map<string,GLuint> shaders;

void printLog(GLuint obj)
{
	int infologLength = 0;
	int maxLength;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	char infoLog[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
		fprintf(stderr,"%s\n",infoLog);

}

GLuint loadShader(string s,GLenum type){
string shad;

ifstream in(s.c_str());
      string line;
      while ( in.good() ){
      getline (in,line);
      shad+=line+"\n";
        }
    in.close();

GLchar* vShad = new char [shad.length()+1];
  strcpy (vShad, shad.c_str());

GLuint vs;
vs = glCreateShader(type);
	glShaderSource(vs, 1, (const GLchar**)(&vShad), NULL);
	glCompileShader(vs);
	printLog(vs);

delete[] vShad;

return vs;
}

#ifndef __scanDirectory
#define __scanDirectory
    void scanDirectory(vector<string> &files, string dirr){

        DIR *dir;
    struct dirent *ent;
    /* open directory stream */
    dir = opendir (dirr.c_str());
    if (dir != NULL) {

      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
       // switch (ent->d_type) {
      //  case DT_REG:
          //printf ("%*.*s\n", ent->d_namlen, ent->d_namlen, ent->d_name);
          files.push_back(ent->d_name);
      //    break;

      //  case DT_DIR:
          //printf ("%s (dir)\n", ent->d_name);
      //    break;

//        default:
 //       break;
          //printf ("%s:\n", ent->d_name);
 //       }
      }

      closedir (dir);
    } else {
      /* could not open directory */
      cerr<<"Could not open "<<dirr<<" Folder!";
    }


        }
#endif

void setupShaders(){

multimap<string,string> shaderPrograms;
vector<string> files;
scanDirectory(files,"Shaders");

for(int x=0;x<files.size();x++){
//cout<<files.at(x)<<endl;

int temp=files.at(x).find(".");
string id=files.at(x).substr(0,temp);

shaderPrograms.insert(pair<string,string>(id,files.at(x)));
}

GLuint prog=0, shad=0;
string last="";
for (multimap<string,string>::iterator it=shaderPrograms.begin(); it!=shaderPrograms.end(); ++it){
    //std::cout << (*it).first << " => " << (*it).second << '\n';
    if(it->second[0]=='.')
    continue;
    if(it->first!=last){
    if(last!=""){

        glBindAttribLocation(prog,0,"in_Position");
        glBindAttribLocation(prog,1,"in_Color");
        glBindAttribLocation(prog,2,"in_Tex");
        glBindAttribLocation(prog,3,"in_Norm");
        glBindAttribLocation(prog,4,"in_data");
        glBindAttribLocation(prog,5,"in_pos2");

    glLinkProgram(prog);
	printLog(prog);

	//printf("%s",gluErrorString(glGetError()));
    }
    prog=glCreateProgram();
    shaders[it->first]=prog;
    last=it->first;
    }

    if(it->second.find("vert")!=string::npos)
    shad=loadShader("Shaders/"+it->second,GL_VERTEX_SHADER);
    else if(it->second.find("frag")!=string::npos)
    shad=loadShader("Shaders/"+it->second,GL_FRAGMENT_SHADER);
    else if(it->second.find("geom")!=string::npos)
    shad=loadShader("Shaders/"+it->second,GL_GEOMETRY_SHADER);
    //
    //else if(it->second.find("comp")!=string::npos)
    //shad=loadShader("Shaders/"+it->second,GL_COMPUTE_SHADER);
    else if(it->second.find("tess")!=string::npos)
    shad=loadShader("Shaders/"+it->second,GL_TESS_CONTROL_SHADER);
    else if(it->second.find("eval")!=string::npos)
    shad=loadShader("Shaders/"+it->second,GL_TESS_EVALUATION_SHADER);


    glAttachShader(prog,shad);
    //printf("%s\n",gluErrorString(glGetError()));
    subShaders.push_back(shad);
}

//link last program
if(last!=""){

        glBindAttribLocation(prog,0,"in_Position");
        glBindAttribLocation(prog,1,"in_Color");
        glBindAttribLocation(prog,2,"in_Tex");
        glBindAttribLocation(prog,3,"in_Norm");
        glBindAttribLocation(prog,4,"in_data");
        glBindAttribLocation(prog,5,"in_pos2");
        glLinkProgram(prog);
        printLog(prog);

    }

//printf("%s\n",gluErrorString(glGetError()));
for(map<string,GLuint>::iterator it=shaders.begin();it!=shaders.end();it++){
cout<<"loaded program "<<it->first<<" : "<<it->second<<endl;
printLog(prog);
}


}

GLuint getShader(string s){
return shaders[s];
}

void setupVAO(GLfloat *texCoords, int texSize, GLfloat *vertexCoords, int verSize, GLuint &vao, GLuint (&vbo)[2]){

glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(2,vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*verSize,vertexCoords,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*texSize,texCoords,GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void cleanupShaders(){

for(map<string,GLuint>::iterator it=shaders.begin();it!=shaders.end();it++)
    glDeleteProgram(it->second);

for(int x=0;x<subShaders.size();x++)
glDeleteShader(subShaders.at(x));


}
