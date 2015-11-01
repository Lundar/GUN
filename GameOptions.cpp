#include "GameOptions.h"
#include <sstream>
GameOptions gameOptions("settings.ini");

void GameOptions::processINILine(string s){

	int eq=s.find('=',0);
	string mod=s.substr(0,eq);
	string mew=s.substr(eq+1);


	/*if(mod=="XRES")
		RESOLUTION_X=atoi(mew.c_str());
	else if(mod=="YRES")
		RESOLUTION_Y=atoi(mew.c_str());
	else if(mod=="FULLSCREEN")
		fullScreen=mew=="true";
    else*/
        properties[mod]=mew;

}

GameOptions::GameOptions(string s){

	ifstream inFile(s.c_str());
	std::string temp;
	while(inFile){

		getline(inFile,temp);
		if(temp[0]!='#')
			processINILine(temp);
	}

}

GameOptions::GameOptions(){}

int GameOptions::getInt(string s, int def){
    if(properties.count(s))
    return atoi(properties[s].c_str());
    return def;
}

bool GameOptions::getBool(string s, bool def){
	if(properties.count(s))
    return properties[s]=="true";
    return def;
}

float GameOptions::getFloat(string s, float def){
    if(properties.count(s))
    return atof(properties[s].c_str());
    return def;
}

string GameOptions::getString(string s, string def){
	if(properties.count(s))
    return properties[s];
    return def;
}

SDL_Keycode GameOptions::getKey(string s, SDL_Keycode def){
	if(properties.count(s))
    return SDL_GetKeyFromName(properties[s].c_str());
    return def;
}

KeyCombo GameOptions::getKeyCombo(string s, SDL_Keycode def){
    KeyCombo ret = {0};
	if(properties.count(s))
        ret.key=SDL_GetKeyFromName(properties[s].c_str());
    else
        ret.key=def;
    return ret;
}

void GameOptions::setString(string s, string s2){
    properties[s]=s2;
}

void GameOptions::setKey(string s, SDL_Keycode c){
    properties[s]=string(SDL_GetKeyName(c));
}

void GameOptions::setInt(string s, int i){
    stringstream ss;
    ss<<i;
    properties[s]=ss.str();
}

void GameOptions::setBool(string s, bool b){
    properties[s]=b?"true":"false";
}

void GameOptions::setFloat(string s, float f){
    stringstream ss;
    ss<<f;
    properties[s]=ss.str();
}

void GameOptions::writeOptions(string s){

    ofstream out(s.c_str());
	std::string temp;
	for(map<string,string>::iterator it=properties.begin();it!=properties.end();it++){

	if(!it->second.empty())
	out<<it->first<<"="<<it->second<<endl;
	}

    out.close();

}
