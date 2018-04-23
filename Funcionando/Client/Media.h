#ifndef MEDIA_H
#define MEDIA_H
#include <string.h>
#include <iostream>
class Media
{
    public:
        Media();
        Media(std::string,int);
        std::string getName();
        int getSizeFile();
        virtual ~Media();

    protected:

    private:
        std::string name;
        int sizeFile;

};

#endif // MEDIA_H
