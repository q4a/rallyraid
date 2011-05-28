#ifndef WSTRINGCONVERTER_H
#define WSTRINGCONVERTER_H

#include <string>
#include <sstream>
#include <irrlicht.h>

class WStringConverter
{
public:
    static void WStringConverter::toString(const irr::core::stringw& wstr, std::string& ret)
    {
        const wchar_t* orig = wstr.c_str();
        toString(orig, ret);
        //ret.resize(origsize);
    }

    static void WStringConverter::toString(const wchar_t* orig, std::string& ret)
    {
        size_t convertedSize;
        size_t origsize = wcslen(orig) + 1;
        char* tmp = new char[origsize];
        wcstombs_s(&convertedSize, tmp, origsize, orig, origsize);
        ret = tmp;
        delete tmp;
        //ret.resize(origsize);
    }

    static void WStringConverter::toInt(const irr::core::stringw& wstr, int& ret)
    {
        const wchar_t* orig = wstr.c_str();
        toInt(orig, ret);
    }

    static void WStringConverter::toInt(const wchar_t* orig, int& ret)
    {
        std::string val;
        toString(orig, val);
        std::istringstream str(val);
        str >> ret;
    }
};

#endif // WSTRINGCONVERTER_H
