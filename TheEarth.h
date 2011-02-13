
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

class TheEarth : public MyThread
{
public:
    static void initialize();
    static void finalize();
    
    static TheEarth* getInstance() {return theEarth;}
    
private:
    static TheEarth* theEarth;
    static const irr::video::SColor baseColor;
    
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
    
public:
    unsigned short getTileHeight(unsigned int x, unsigned int y);
    const irr::video::SColor& getTileTexture(unsigned int x, unsigned int y);

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
        
        Terrain* terrainCircle[3][3];
        static const irr::core::vector3di terrainPos[3][3];
        static const irr::core::vector3df terrainPosf[3][3];
    };
    
    VisualMembers*      visualPart;
    VisualMembers*      newVisualPart;
    
    irr::core::aabbox3df    lastPosBox;
    irr::core::vector3df    lastCenterPos;
    irr::core::vector3di    lastCenterPosi;

    
    
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
