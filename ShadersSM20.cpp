
#include <irrlicht.h>
#include <Cg/cg.h>
#include <set>
#include <assert.h>

#include "ShadersSM20.h"
#include "IrrCg.h"
#include "TheGame.h"
#include "ConfigFile.h"

using namespace IrrCg;


class SM20_ShaderCallback : public ICgShaderConstantSetCallBack
{
public:
    SM20_ShaderCallback()
        : device(TheGame::getInstance()->getDevice()), driver(TheGame::getInstance()->getDriver())
    {
        callbacks.insert(this);
    }

    virtual void OnSetConstants(ICgServices* services,const CGeffect& Effect,const CGpass& Pass,const CGprogram& Vertex,const CGprogram& Pixel,const irr::video::SMaterial& Material)
    {
        //assert(0);
    }

public:
    irr::IrrlichtDevice*        device;
    irr::video::IVideoDriver*   driver;

public:
    static std::set<SM20_ShaderCallback*> callbacks;
};


/*
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
*/

std::set<SM20_ShaderCallback*> SM20_ShaderCallback::callbacks;

ShadersSM20::ShadersSM20()
    : Shaders()
{
    if (supportedSMVersion < 2)
    {
        return;
    }
    
    loadSM20Materials();
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

void ShadersSM20::loadSM20Materials()
{
    // Load resource paths from config file
    //materialMap.clear();

    const irr::c8* ogl_vs_version = "arbvp1";
    const irr::c8* ogl_ps_version = "arbfp1";
    const irr::c8* d3d_vs_version = "vs_2_0";
    const irr::c8* d3d_ps_version = "ps_2_0";


    ConfigFile cf;
    cf.load("data/materials/sm20.cfg");

    dprintf(MY_DEBUG_NOTE, "Read SM20 materials file:\n");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string materialName, keyName, typeName, shaderFile;
    while (seci.hasMoreElements())
    {
        shaderFile = "";
        materialName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tMaterial: %s\n", materialName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        for (ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            typeName = i->second;

            if (keyName == "shader")
            {
                shaderFile = typeName;
            }
        }
        
        
        if (materialName != "" && shaderFile != "")
        {
            irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID;
            materialMap_t::const_iterator it = materialMap.find(materialName);
            if (it != materialMap.end())
            {
                baseMaterial = it->second;
            }
            //if (materialName=="blabla")
            //{
            //    use specific shader CB
            //}
            //else
            {
                materialMap[materialName] = (irr::video::E_MATERIAL_TYPE)gpu->addCgShaderMaterialFromFiles(CG_SOURCE,
                    shaderFile.c_str(), "main_v", ogl_vs_version, d3d_vs_version,
                    shaderFile.c_str(), "main_f", ogl_ps_version, d3d_ps_version,
                    new SM20_ShaderCallback(), /*video::EMT_SOLIDirr::video::EMT_TRANSPARENT_ALPHA_CHANNEL*/baseMaterial);
            }
        }
    }
}

