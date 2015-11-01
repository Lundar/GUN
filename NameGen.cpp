#include "NameGen.h"
#include <fstream>
#include <stdint.h>
#include <stdlib.h>

#define END_CHAR 127
#define BEGIN_CHAR 0

NameGen::NameGen(string file){
    train(file);
}

NameGen::~NameGen(){
    //dtor
}

uint16_t makePair(char c1, char c2){
    uint16_t ret;
    ((char*)(&ret))[0]=c1;
    ((char*)(&ret))[1]=c2;
    return ret;
}

void NameGen::train(string f){

    ifstream in(f.c_str());
    char name[256];

    while(in.good()){
        in.getline(name,256);

        mark[makePair(BEGIN_CHAR,BEGIN_CHAR)][name[0]]++;
        mark[makePair(BEGIN_CHAR,name[0])][name[1]]++;

        int x;
        for(x=2;name[x];x++)
            mark[makePair(name[x-2],name[x-1])][name[x]]++;

        mark[makePair(name[x-2],name[x-1])][END_CHAR]++;
        mark[makePair(name[x-1],END_CHAR)][END_CHAR]++;

    }

}

string NameGen::genName(int max){

    map<char,uint32_t> *chain=NULL;
    string out;
    char last[2];
    last[0]=BEGIN_CHAR;
    last[1]=BEGIN_CHAR;
    char tmp=END_CHAR;

    for(int i=0;i<max;i++){
        chain = &(mark[makePair(last[0],last[1])]);

        uint64_t tot=0;
        for(map<char,uint32_t>::iterator it=chain->begin();it!=chain->end();it++)
            tot+=it->second;
        if(tot==0)
            return "error";
        int64_t r = rand()%tot;
        for(map<char,uint32_t>::iterator it=chain->begin();it!=chain->end();it++){
            r-=it->second;
            if(r<0){
                tmp=it->first;
                break;
            }
        }

        if(tmp==END_CHAR)
            break;

        out+=tmp;
        last[0]=last[1];
        last[1]=tmp;
    }

    return out;
}
