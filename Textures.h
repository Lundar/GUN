#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED

#include <map>
#include <vector>
#include <string>
#include <GL/glew.h>

using namespace std;
//GLuint defaultTex;
//extern map<string,GLuint> loadedTextures;
void scanDirectory(vector<string> &files, string dir);

bool is2Pwr(int x);

GLuint loadPNG(const char * file_name, GLuint tex);

void loadPicture(std::string file, GLuint tex);

void loadTextures();

void unloadTextures();


GLuint getTexture(std::string s);

///TODO
//modernize text renderer
class TextRenderer{

	public:

		TextRenderer(string tex){
			charSheet=getTexture(tex/*"TextW"*/);
			charsPerLine=30;
			linesPerSheet=19;
			charWidth=.995/charsPerLine;
			charHeight=.995/linesPerSheet;
            text=tex;
			renderHeight=charHeight*10;
			renderWidth=charWidth*10;
		}

		TextRenderer(string tex,string t, GLuint shad, int cpl, int lps, float offx, float offy){
			charSheet=getTexture(t);
			charsPerLine=cpl;
			linesPerSheet=lps;
			charWidth=(1-offx)/charsPerLine;
			charHeight=(1-offy)/linesPerSheet;
            text=tex;
			renderHeight=charHeight;
			renderWidth=charWidth;
		}

		void renderString(string s,float offset=0);
		void renderToBox(string text,float width, float height);
		void getTexCoords(char c, float *ret);
		void setRenderSize(float x,float y){renderHeight=y,renderWidth=x;}

	protected:
		GLuint charSheet;
		float charWidth;
		float charHeight;
		int charsPerLine;
		int linesPerSheet;
		float renderWidth;
		float renderHeight;
		string text;
        GLuint vao;
        GLuint buff[2];

};


/*void TextRenderer::renderToBox(string text,float width, float height){

	int maxChars=width/renderWidth;
	int maxLines=height/renderHeight;
	int cnt=0;

	for(int x=0;x<text.length();x+=maxChars){
		//renderString(text.substr(x,maxChars)+(text.length()>x+maxChars-1&&text.at(x+maxChars-1)!=' '?"-":""),-cnt*renderHeight);

		renderString(text.substr(begin,x-begin),-cnt*renderHeight);

		cnt++;
		if(cnt>maxLines)
			break;
	}
}*/
#endif // TEXTURES_H_INCLUDED
