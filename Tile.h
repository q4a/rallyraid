#ifndef TILE_H
#define TILE_H

#include "Terrain_defs.h"
#include <irrlicht.h>

class Tile
{
public:
    Tile(unsigned int posx, unsigned int posy,
        const irr::video::SColor& color00,
        const irr::video::SColor& color10,
        const irr::video::SColor& color01,
        const irr::video::SColor& color11,
        const irr::video::SColor& density,
        bool autoRead = true);
    ~Tile();

    unsigned short getHeight(unsigned int x, unsigned int y) const
    {
        if (height && x < TILE_POINTS_NUM && y < TILE_POINTS_NUM)
        {
            return height[x + (TILE_POINTS_NUM*y)];
        }
        else
        {
            return 0;
        }
    }
    
    const irr::video::SColor& getColor(unsigned int x, unsigned int y) const
    {
        if (colors && x < TILE_POINTS_NUM && y < TILE_POINTS_NUM)
        {
            return colors[x + (TILE_POINTS_NUM*y)];
        }
        else
        {
            return baseColor;
        }
    }

    const irr::video::SColor& getFineColor(unsigned int x, unsigned int y) const
    {
        if (fineColors && x < TILE_FINE_POINTS_NUM && y < TILE_FINE_POINTS_NUM)
        {
            return fineColors[x + (TILE_FINE_POINTS_NUM*y)];
        }
        else
        {
            return baseColor;
        }
    }
    
    const irr::video::SColor& getFineDensity(unsigned int x, unsigned int y) const
    {
        if (fineDensity && x < TILE_FINE_POINTS_NUM && y < TILE_FINE_POINTS_NUM)
        {
            return fineDensity[x + (TILE_FINE_POINTS_NUM*y)];
        }
        else
        {
            return baseColor;
        }
    }
    
    void setFineColor(unsigned int x, unsigned int y, const irr::video::SColor& val = irr::video::SColor())
    {
        if (fineColors && x < TILE_FINE_POINTS_NUM && y < TILE_FINE_POINTS_NUM)
        {
            fineColors[x + (TILE_FINE_POINTS_NUM*y)] = val;
        }
    }
    
    void setFineDensity(unsigned int x, unsigned int y, const irr::video::SColor& val = irr::video::SColor())
    {
        if (fineDensity && x < TILE_FINE_POINTS_NUM && y < TILE_FINE_POINTS_NUM)
        {
            fineDensity[x + (TILE_FINE_POINTS_NUM*y)] = val;
        }
    }
    
    bool getInUse() {return inUse;}
    bool isInUse() {return inUse;}
    void setInUse() {inUse = true;}
    void clearInUse() {inUse = false;}

    unsigned int getPosX() {return posx;}
    unsigned int getPosY() {return posy;}

private:
    bool setHeight(unsigned int x, unsigned int y, unsigned short val);
    bool read();


private:
    unsigned int catx;
    unsigned int caty;
    unsigned int posx;
    unsigned int posy;

    unsigned short* height;
    bool inUse;
    irr::video::SColor* colors;
    irr::video::SColor* fineColors;
    irr::video::SColor* fineDensity;

    static const irr::video::SColor baseColor;
};

#endif // TILE_H
