#ifndef WSTRINGCONVERTER_H
#define WSTRINGCONVERTER_H

#include <string>
//#include <sstream>
#include <irrlicht.h>

class WStringConverter
{
public:
    static void WStringConverter::toString(const irr::core::stringw& wstr, std::string& ret)
    {
        const wchar_t* orig = str.c_str();
        size_t origsize = wcslen(orig) + 1;
        char* tmp = new char[origsize];
        wcstombs(tmp, orig, origsize);
        ret = tmp;
        delete tmp;
        //ret.resize(origsize);
    }

};

#endif // WSTRINGCONVERTER_H
