
#ifndef THEEARTH_H
#define THEEARTH_H

#include <set>

namespace irr
{
    namespace video
    {
        class IImage;
        class SColor;
    }
}

class Tile;


class TheEarth
{
public:
    static void initialize();
    static void finalize();
    
    static TheEarth* getInstance() {return theEarth;}
    
private:
    static TheEarth* theEarth;
    
    typedef std::set<Tile*> tileSet_t;

public:
    static const unsigned int TILE_POINTS_NUM = 32;
    static const unsigned int TILE_SCALE = 64;
    static const unsigned int TILE_SIZE = TILE_POINTS_NUM * TILE_SCALE;

private:
    TheEarth();
    ~TheEarth();

    bool setHeight(unsigned int x, unsigned int y, unsigned short val);
    bool setHasDetail(unsigned int x, unsigned int y, bool val);
    bool setIsLoaded(unsigned int x, unsigned int y, bool val);
    bool setHasDetailTex(unsigned int x, unsigned int y, bool val);
    bool setDensity(unsigned int x, unsigned int y, const irr::video::SColor& val);
    
public:
    unsigned short getHeight(unsigned int x, unsigned int y) const;
    bool getHasDetail(unsigned int x, unsigned int y) const;
    bool getIsLoaded(unsigned int x, unsigned int y) const;
    bool getHasDetailTex(unsigned int x, unsigned int y) const;
    const irr::video::SColor& getDensity(unsigned int x, unsigned int y) const;
    const irr::video::SColor& getTexture(unsigned int x, unsigned int y) const;

    bool read();
    bool readHeight();
    bool readHasDetail();
    bool readHasDetailTex();
    bool readDensity();
    bool readEarthTexture();
    
    bool write();
    bool writeHeight();
    bool writeHasDetail();
    bool writeHasDetailTex();
    
    bool writeToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const;
    bool writeHeightToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const;
    bool writeHasDetailToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const;

    bool writeDensityToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const;
    bool writeEarthTextureToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const;


private:
    // common data
    
    // visual data
    irr::video::IImage* earthTexture;
    
    // store data
    unsigned int        xsize;
    unsigned int        ysize;

    unsigned short*     height;
    irr::video::IImage* density;
    unsigned char*      hasDetail;
    unsigned char*      isLoaded;
    unsigned char*      hasDetailTex;

    unsigned short      maxHeight;
    unsigned short      minHeight;
    
    tileSet_t           tileSet;
};

#endif // THEEARTH_H
