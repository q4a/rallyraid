
#include <irrlicht.h>
#include <Cg/cg.h>
#include <set>
#include <assert.h>

#include "ShadersSM20.h"
#include "IrrCg.h"
#include "TheGame.h"

using namespace IrrCg;


class SM20_ShaderCallback : public ICgShaderConstantSetCallBack
{
public:
    SM20_ShaderCallback()
        : device(TheGame::getInstance()->getDevice()), driver(TheGame::getInstance()->getDriver())
    {
    }

    virtual void OnSetConstants(ICgServices* services,const CGeffect& Effect,const CGpass& Pass,const CGprogram& Vertex,const CGprogram& Pixel,const irr::video::SMaterial& Material)
    {
        assert(0);
    }

public:
    irr::IrrlichtDevice*        device;
    irr::video::IVideoDriver*   driver;

public:
    static std::set<SM20_ShaderCallback*> callbacks;
};


class SM20_ShaderCallback_quad2d : public SM20_ShaderCallback
{
public:
    //CGparameter tex0;
public:
    SM20_ShaderCallback_quad2d()
    {

    }

    virtual void OnSetConstants(ICgServices* services,const CGeffect& Effect,const CGpass& Pass,const CGprogram& Vertex,const CGprogram& Pixel,const irr::video::SMaterial& Material)
    {
    }

};

std::set<SM20_ShaderCallback*> SM20_ShaderCallback::callbacks;

ShadersSM20::ShadersSM20()
    : Shaders()
{
    if (supportedSMVersion < 2)
    {
        return;
    }
    const irr::c8* ogl_vs_version = "arbvp1";
    const irr::c8* ogl_ps_version = "arbfp1";
    const irr::c8* d3d_vs_version = "vs_2_0";
    const irr::c8* d3d_ps_version = "ps_2_0";

    irr::core::stringc cg_dir = "data/shaders/cg/SM20/";

    irr::core::stringc quad2dFilename = cg_dir + "quad2d.cg";

    quad2d = (irr::video::E_MATERIAL_TYPE)gpu->addCgShaderMaterialFromFiles(CG_SOURCE,
                    quad2dFilename.c_str(), "main_v", ogl_vs_version, d3d_vs_version,
                    quad2dFilename.c_str(), "main_f", ogl_ps_version, d3d_ps_version,
                    new SM20_ShaderCallback_quad2d(), /*video::EMT_SOLID*/irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
}

ShadersSM20::~ShadersSM20()
{
    for (std::set<SM20_ShaderCallback*>::iterator it = SM20_ShaderCallback::callbacks.begin();
         it != SM20_ShaderCallback::callbacks.end();
         it++)
    {
        delete *it;
    }
}
