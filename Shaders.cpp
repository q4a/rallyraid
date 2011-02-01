
#include <irrlicht.h>
#include <Cg/cg.h>

#include "Shaders.h"
#include "IrrCg.h"
#include "TheGame.h"

using namespace IrrCg;

Shaders::Shaders()
    : gpu(0),
      supportedSMVersion(0),
      quad2d(irr::video::EMT_SOLID)
{
    gpu = new ICgProgrammingServices(TheGame::getInstance()->getDevice()/*, false, true*/);
    ICgServices* cgServices = gpu->getCgServices();

    printf("CG support:\n");
    printf("arbfp1:    %d\n", cgServices->isProfileSupported("arbfp1"));
    printf("arbvp1:    %d\n", cgServices->isProfileSupported("arbvp1"));
    printf("fp20:      %d\n", cgServices->isProfileSupported("fp20"));
    printf("fp30:      %d\n", cgServices->isProfileSupported("fp30"));
    printf("fp40:      %d\n", cgServices->isProfileSupported("fp40"));
    printf("glslf:     %d\n", cgServices->isProfileSupported("glslf"));
    printf("glslg:     %d\n", cgServices->isProfileSupported("glslg"));
    printf("glslv:     %d\n", cgServices->isProfileSupported("glslv"));
    printf("gp4:       %d\n", cgServices->isProfileSupported("gp4"));
    printf("gp4fp:     %d\n", cgServices->isProfileSupported("gp4fp"));
    printf("gp4gp:     %d\n", cgServices->isProfileSupported("gp4gp"));
    printf("gp4vp:     %d\n", cgServices->isProfileSupported("gp4vp"));
    printf("hlslf:     %d\n", cgServices->isProfileSupported("hlslf"));
    printf("hlslv:     %d\n", cgServices->isProfileSupported("hlslv"));
    printf("ps_1_1:    %d\n", cgServices->isProfileSupported("ps_1_1"));
    printf("ps_1_2:    %d\n", cgServices->isProfileSupported("ps_1_2"));
    printf("ps_1_3:    %d\n", cgServices->isProfileSupported("ps_1_3"));
    printf("ps_2_0:    %d\n", cgServices->isProfileSupported("ps_2_0"));
    printf("ps_2_x:    %d\n", cgServices->isProfileSupported("ps_2_x"));
    printf("ps_3_0:    %d\n", cgServices->isProfileSupported("ps_3_0"));
    printf("ps_4_0:    %d\n", cgServices->isProfileSupported("ps_4_0"));
    printf("vp20:      %d\n", cgServices->isProfileSupported("vp20"));
    printf("vp30:      %d\n", cgServices->isProfileSupported("vp30"));
    printf("vp40:      %d\n", cgServices->isProfileSupported("vp40"));
    printf("vs_4_0:    %d\n", cgServices->isProfileSupported("vs_4_0"));
    printf("gs_4_0:    %d\n", cgServices->isProfileSupported("gs_4_0"));

    irr::video::E_DRIVER_TYPE driverType = TheGame::getInstance()->getDriver()->getDriverType();

    if ((driverType == irr::video::EDT_DIRECT3D9 && TheGame::getInstance()->getDriver()->queryFeature(irr::video::EVDF_HLSL) &&
         TheGame::getInstance()->getDriver()->queryFeature(irr::video::EVDF_PIXEL_SHADER_2_0) && TheGame::getInstance()->getDriver()->queryFeature(irr::video::EVDF_VERTEX_SHADER_2_0)
        ) ||
        ((driverType == irr::video::EDT_OPENGL /*|| driverType == video::EDT_OPENGL3*/) && TheGame::getInstance()->getDriver()->queryFeature(irr::video::EVDF_ARB_GLSL) &&
         TheGame::getInstance()->getDriver()->queryFeature(irr::video::EVDF_ARB_FRAGMENT_PROGRAM_1) && TheGame::getInstance()->getDriver()->queryFeature(irr::video::EVDF_ARB_VERTEX_PROGRAM_1)
        )
       )
    {
        supportedSMVersion = 2;
    }
}

Shaders::~Shaders()
{
    if (gpu)
    {
        delete gpu;
        gpu = 0;
    }
}
