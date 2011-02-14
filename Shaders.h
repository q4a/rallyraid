
#ifndef SHADERS_H
#define SHADERS_H

#include <irrlicht.h>
#include <map>
#include <string>

namespace IrrCg
{
    class ICgProgrammingServices;
}


class Shaders
{
public:
    typedef std::map<std::string, irr::video::E_MATERIAL_TYPE> materialMap_t;

    Shaders();
    ~Shaders();

    unsigned int getSupportedSMVersion() {return supportedSMVersion;}

    static irr::video::E_MATERIAL_TYPE stringToBaseType(const std::string& baseMaterialName);

private:
    void loadBaseMaterials();
    
protected:
    IrrCg::ICgProgrammingServices*  gpu;
    unsigned int                    supportedSMVersion;

// materials
public:
    materialMap_t                   materialMap;

};

#endif // SHADERS_H
