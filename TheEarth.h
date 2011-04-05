
#ifndef THEEARTH_H
#define THEEARTH_H

#include <irrlicht.h>
#include <map>
#include "Tile.h"
#include "MyThread.h"

namespace irr
{
    namespace video
    {
        class IImage;
        class SColor;
    }
}
class Terrain;

#define MINIMAP_SIZE 128
#define MINIMAP_HSIZE (MINIMAP_SIZE / 2)

class TheEarth : public MyThread
{
public:
    static void initialize();
    static void finalize();
    
    static TheEarth* getInstance() {return theEarth;}
    
private:
    static TheEarth* theEarth;
    static irr::video::SColor baseColor;
    
public:
    typedef std::map<unsigned int, Tile*> tileMap_t;

private:
    TheEarth();
    ~TheEarth();

    virtual void run();

    bool setEarthHeight(unsigned int x, unsigned int y, unsigned short val);
    bool setHasDetail(unsigned int x, unsigned int y, bool val);
    bool setIsLoaded(unsigned int x, unsigned int y, bool val);
    bool setHasDetailTex(unsigned int x, unsigned int y, bool val);
    bool setEarthDensity(unsigned int x, unsigned int y, const irr::video::SColor& val);
    void refreshMiniMap();
    
public:
    unsigned short getTileHeight(unsigned int x, unsigned int y);
    const irr::video::SColor& getTileTexture(unsigned int x, unsigned int y);
    void getTileHeightAndTexture(unsigned int x, unsigned int y,
        unsigned short& height, irr::video::SColor& textureColor);
    const irr::video::SColor& getTileFineTexture(unsigned int x, unsigned int y); // x and y is not devided by TILE_SCALE, but TILE_FINE_SCALE
        
    unsigned short getEarthHeight(unsigned int x, unsigned int y) const;
    bool getHasDetail(unsigned int x, unsigned int y) const;
    bool getIsLoaded(unsigned int x, unsigned int y) const;
    bool getHasDetailTex(unsigned int x, unsigned int y) const;
    irr::video::SColor getEarthDensity(unsigned int x, unsigned int y) const;
    irr::video::SColor getEarthTexture(unsigned int x, unsigned int y) const;

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

    void createFirst(const irr::core::vector3df& pos, const irr::core::vector3df& dir);
    void update(const irr::core::vector3df& pos, const irr::core::vector3df& dir);
    void registerVisual();

    irr::video::ITexture* getMiniMapTexture() {return miniMapTexture;}

    float getHeight(float x, float z);
    float getHeight(const irr::core::vector3df& pos);
    float getHeight(const irr::core::vector2df& pos);
    
    unsigned int getSizeX() {return xsize;}
    unsigned int getSizeY() {return ysize;}

    const tileMap_t& getTileMap() {return tileMap;}

private:
    // common data
    
    // visual data - real Z coord
    irr::video::IImage* earthTexture;
    struct VisualMembers
    {
        VisualMembers();
        ~VisualMembers();
        
        void setVisible(bool visible);
        void createMembers(const irr::core::vector3di& centerPosi, TheEarth* earth);
        void loadMembers(TheEarth* earth);
        void finalizeMembers();
        void registerMembers();
        float getHeight(float x, float z);
        
        Terrain* terrainCircle[3][3];
        static const irr::core::vector3di terrainPos[3][3];
        static const irr::core::vector3df terrainPosf[3][3];
    };
    
    VisualMembers*      visualPart;
    VisualMembers*      newVisualPart;
    VisualMembers*      newReadyVisualPart;
    
    irr::core::aabbox3df    lastPosBox;
    irr::core::vector3df    lastCenterPos;
    irr::core::vector3di    lastCenterPosi;

    irr::video::IImage*     miniMap;
    irr::video::ITexture*   miniMapTexture;
    irr::core::dimension2di lastMiniMapPos;
    
    
    // store data - reveresed Z coord
    unsigned int        xsize;
    unsigned int        ysize;

    unsigned short*     height;
    irr::video::IImage* density;
    unsigned char*      hasDetail;
    unsigned char*      isLoaded;
    unsigned char*      hasDetailTex;

    unsigned short      maxHeight;
    unsigned short      minHeight;
    
    tileMap_t           tileMap;

};

#endif // THEEARTH_H
