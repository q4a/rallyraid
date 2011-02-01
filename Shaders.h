
#ifndef SHADERS_H
#define SHADERS_H

#include <irrlicht.h>

namespace IrrCg
{
    class ICgProgrammingServices;
}


class Shaders
{
public:
    Shaders();
    ~Shaders();

protected:
    IrrCg::ICgProgrammingServices*  gpu;
    unsigned int                    supportedSMVersion;

// materials
public:
    irr::video::E_MATERIAL_TYPE     quad2d;

};

#endif // SHADERS_H
