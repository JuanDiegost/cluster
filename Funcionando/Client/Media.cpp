#include "Media.h"

Media::Media()
{
    //ctor
}

Media::Media(std::string name,int sizeFile){
    this->name=name;
    this->sizeFile=sizeFile;
}

int Media::getSizeFile(){
    return this->sizeFile;
}

std::string Media::getName(){
    return this->name;
}

Media::~Media()
{
    //dtor
}

