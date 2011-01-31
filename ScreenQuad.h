#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include <irrlicht.h>

class ScreenQuad
{
public:
    ScreenQuad(bool pflip_vert)
    {
        Material.Wireframe = false;
        Material.Lighting = false;
        Material.ZWriteEnable = false;
        Material.BackfaceCulling = false;
        Material.ZBuffer = ECFN_ALWAYS/*ECFN_NEVER*/;
        //Material.setFlag(EMF_BILINEAR_FILTER, false);
        
        if (pflip_vert)
        {
            Vertices[0] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,0.0f);
            Vertices[1] = irr::video::S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
            Vertices[2] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,1.0f);
            Vertices[3] = irr::video::S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
        }
        else
        {
            Vertices[0] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
            Vertices[1] = irr::video::S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,0.0f);
            Vertices[2] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
            Vertices[3] = irr::video::S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,1.0f);
        }
    }

    virtual void render(irr::video::IVideoDriver* driver)
    {
        irr::u16 indices[6] = {0, 1, 2, 3, 0, 2};

        driver->setMaterial(Material);
        driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
        driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 2);
        driver->setMaterial(irr::video::SMaterial());
    }

    virtual irr::video::SMaterial& getMaterial()
    {
        return Material;
    }

    void set2DVertexPos(int index, const irr::core::position2d<irr::s32>& newPos)
    {
        irr::core::dimension2d<irr::u32> screenSize;
        Vertices[index].Pos.X = 2.f * ((float)newPos.X / (float)screenSize.Width) - 1.f;
        Vertices[index].Pos.Y = (1.f - ((float)newPos.Y / (float)screenSize.Height)) * 2.f - 1.f;
    }

private:
    irr::video::S3DVertex Vertices[4];
    irr::video::SMaterial Material;
};

#endif
