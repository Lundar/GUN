#include "Textures.h"
#include <dirent.h>
#include <map>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <png.h>
#include <SDL2/SDL.h>


//GLuint defaultTex;
map<string,GLuint> loadedTextures;

GLuint loadPNG(const char * file_name, GLuint tex)
{
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    GLint format;
    switch(color_type)
    {
    case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
        break;
    case PNG_COLOR_TYPE_GRAY:
        //cout<<"color Type Grey"<<endl;
        format = GL_RGBA;
        png_set_gray_to_rgb(png_ptr);
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

        break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        //cout<<"color Type Grey Alpha"<<endl;
        format = GL_RGBA;
        png_set_gray_to_rgb(png_ptr);

        break;
    case PNG_COLOR_TYPE_PALETTE:
        //cout<<"color Type Palette"<<endl;
        format = GL_RGBA;
        png_set_palette_to_rgb(png_ptr);
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
        //return 0;
        break;
    default:
        fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
        return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    //image_data = (png_byte*) malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    image_data = new png_byte[rowbytes * temp_height];
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    //png_bytep * row_pointers = (png_bytep*) malloc(temp_height * sizeof(png_bytep));
    png_bytep * row_pointers = new png_bytep[temp_height];
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[/*temp_height - 1 - */i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] image_data;
    delete[] row_pointers;
    fclose(fp);
    return tex;
}

void loadPicture(std::string file, GLuint tex){

    if(file.find(".png")!=string::npos){
        loadPNG(file.c_str(),tex);
        cout<<"Loaded: "<<file<<endl;
    }

	if(file.find(".bmp")==string::npos)
		return;

	cout<<"Loading: "<<file<<endl;


	SDL_Surface *image;
	SDL_Surface *fixed;
	image = SDL_LoadBMP(file.c_str());

	if(!image){
		cerr<<"error loading:"<<file;
		return;
		//system("pause");
	}

	fixed=SDL_ConvertSurfaceFormat(image,SDL_PIXELFORMAT_ABGR8888,0);
    //SDL_SetColorKey(fixed,SDL_TRUE,0xFF00FF);

	if(!fixed){
		cerr<<"error loading:"<<file;
		return;
		//system("pause");
	}



	//if(!(is2Pwr(image->w)&&is2Pwr(image->h)))
	//	cerr<<"image is not power of 2 it may load incorrectly"<<endl;
	// handle error

	glBindTexture(GL_TEXTURE_2D,tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	//if(alpha)
	//glTexImage2D( GL_TEXTURE_2D, 0, 4, image->w, image->h, 0,GL_BGRA, GL_UNSIGNED_BYTE, image->pixels );
	//else
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0,GL_RGBA, GL_UNSIGNED_BYTE, fixed->pixels );

	if ( image ) {
		SDL_FreeSurface( image );
	}

}

void loadTextures(){
	//glGenTextures(1, &defaultTex);

	vector<string> files;
	scanDirectory(files,"Textures");


	GLuint temps[files.size()];
	glGenTextures(files.size(),temps);

	for(int x=0;x<files.size();x++){
		loadPicture("Textures/"+files.at(x),temps[x]);
		loadedTextures[files.at(x).substr(0,files.at(x).size()-4)]=temps[x];
	}

}

void unloadTextures(){

	for(map<string,GLuint>::iterator it=loadedTextures.begin();it!=loadedTextures.end();it++)
		glDeleteTextures(1,&it->second);

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
			files.push_back(ent->d_name);
		}

		closedir (dir);
	} else {
		/* could not open directory */
		cerr<<"Could not open "<<dirr<<" Folder!";
	}


}
#endif

GLuint getTexture(std::string s){
	std::map<string,GLuint>::iterator it= loadedTextures.find(s);
	if(it==loadedTextures.end())
		return loadedTextures["dne"];
	return it->second;
}

void TextRenderer::getTexCoords(char c, GLfloat *ret){

	int row,column;
	row=(unsigned int)(c-1)/charsPerLine;
	column=(unsigned int)(c-1)%charsPerLine;

	(ret)[0]=column*charWidth;
	(ret)[1]=row*charHeight;

	(ret)[2]=column*charWidth;
	(ret)[3]=(row+1)*charHeight;

	(ret)[4]=(column+1)*charWidth;
	(ret)[5]=(row+1)*charHeight;

	(ret)[6]=(column+1)*charWidth;
	(ret)[7]=row*charHeight;
}


void TextRenderer::renderString(string text,float offset){
	glBindTexture(GL_TEXTURE_2D,charSheet);


	GLfloat texCoords[8*text.length()];
	GLfloat vertexCoords[12*text.length()];

	for(int x=0;x<text.length();x++){
		for(int y=0;y<12;y++)
			vertexCoords[x*12+y]=0;

		vertexCoords[x*12]=x*renderWidth;
		vertexCoords[x*12+1]=offset;
		vertexCoords[x*12+2]=-5;
		vertexCoords[x*12+3]=x*renderWidth;
		vertexCoords[x*12+4]=-renderHeight+offset;
		vertexCoords[x*12+5]=-5;
		vertexCoords[x*12+6]=(x+1)*renderWidth;
		vertexCoords[x*12+7]=-renderHeight+offset;
		vertexCoords[x*12+8]=-5;
		vertexCoords[x*12+9]=(x+1)*renderWidth;
		vertexCoords[x*12+10]=offset;
		vertexCoords[x*12+11]=-5;
		//setup texture coords
		getTexCoords(text[x],&texCoords[x*8]);
	}


	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(2,buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*12*text.length(),vertexCoords,GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, buff[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*8*text.length(),texCoords,GL_STREAM_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_QUADS,0,4*text.length());

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(2);
	glBindVertexArray(0);

	glDeleteBuffers(2,buff);
	glDeleteVertexArrays(1,&vao);
	glBindTexture(GL_TEXTURE_2D,0);
}

int getNextWord(string s,int pos,int len){
int x=pos;
while(s[x]==' '&&x<s.length())
x++;

while(s[x]!=' '&&x<s.length())
x++;

//if(x-pos>len)
//return pos+len;

return x;
}

void TextRenderer::renderToBox(string text,float width, float height){

	int maxChars=width/renderWidth;
	int maxLines=height/renderHeight;
	int cnt=0;
	int begin=0;
	int x=0;
	int lx=0;

	while(x<text.length()){
    lx=x;
	x=getNextWord(text,x,maxChars);

	if(x-begin>maxChars){
	renderString(text.substr(begin,lx-begin),-cnt*renderHeight);
	cnt++;
    if(cnt>maxLines)
        break;

	begin=lx+1;

	}

    renderString(text.substr(begin,x-begin),-cnt*renderHeight);


	}

}
