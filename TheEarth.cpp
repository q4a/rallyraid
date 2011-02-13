
#include "TheEarth.h"
#include "Tile.h"
#include "TheGame.h"


#include <irrlicht.h>
#include <stdio.h>
#include <assert.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <direct.h>
#include <errno.h>
#include "Terrain.h"


const irr::core::vector3di TheEarth::VisualMembers::terrainPos[3][3] =
{
    {
        irr::core::vector3di(-TILE_SIZE-TILE_HSIZE, 0,            TILE_HSIZE),
        irr::core::vector3di(-TILE_SIZE-TILE_HSIZE, 0,           -TILE_HSIZE),
        irr::core::vector3di(-TILE_SIZE-TILE_HSIZE, 0, -TILE_SIZE-TILE_HSIZE)

    },
    {
        irr::core::vector3di(          -TILE_HSIZE, 0,            TILE_HSIZE),
        irr::core::vector3di(          -TILE_HSIZE, 0,           -TILE_HSIZE),
        irr::core::vector3di(          -TILE_HSIZE, 0, -TILE_SIZE-TILE_HSIZE)
    },
    {
        irr::core::vector3di(           TILE_HSIZE, 0,            TILE_HSIZE),
        irr::core::vector3di(           TILE_HSIZE, 0,           -TILE_HSIZE),
        irr::core::vector3di(           TILE_HSIZE, 0, -TILE_SIZE-TILE_HSIZE)
    }
};

const irr::core::vector3df TheEarth::VisualMembers::terrainPosf[3][3] =
{
    {
        irr::core::vector3df(-TILE_SIZE_F-TILE_HSIZE_F, 0.0f,              TILE_HSIZE_F),
        irr::core::vector3df(-TILE_SIZE_F-TILE_HSIZE_F, 0.0f,             -TILE_HSIZE_F),
        irr::core::vector3df(-TILE_SIZE_F-TILE_HSIZE_F, 0.0f, -TILE_SIZE_F-TILE_HSIZE_F)

    },
    {
        irr::core::vector3df(            -TILE_HSIZE_F, 0.0f,              TILE_HSIZE_F),
        irr::core::vector3df(            -TILE_HSIZE_F, 0.0f,             -TILE_HSIZE_F),
        irr::core::vector3df(            -TILE_HSIZE_F, 0.0f, -TILE_SIZE_F-TILE_HSIZE_F)
    },
    {
        irr::core::vector3df(             TILE_HSIZE_F, 0.0f,              TILE_HSIZE_F),
        irr::core::vector3df(             TILE_HSIZE_F, 0.0f,             -TILE_HSIZE_F),
        irr::core::vector3df(             TILE_HSIZE_F, 0.0f, -TILE_SIZE_F-TILE_HSIZE_F)
    }
};


TheEarth::VisualMembers::VisualMembers()
{
    memset(terrainCircle, 0, sizeof(Terrain*)*3*3);
}

TheEarth::VisualMembers::~VisualMembers()
{
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            if (terrainCircle[i][j])
            {
                delete terrainCircle[i][j];
                terrainCircle[i][j] = 0;
            }
        }
    }
}

void TheEarth::VisualMembers::setVisible(bool visible)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            if (terrainCircle[i][j])
            {
                terrainCircle[i][j]->setVisible(visible);
            }
        }
    }
}

void TheEarth::VisualMembers::createMembers(const irr::core::vector3di& centerPosi, TheEarth* earth)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            assert(terrainCircle[i][j] == 0);
            terrainCircle[i][j] = new Terrain(centerPosi+terrainPos[i][j], earth);
        }
    }
}

void TheEarth::VisualMembers::loadMembers(TheEarth* earth)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            assert(terrainCircle[i][j] != 0);
        }
    }
}

void TheEarth::VisualMembers::finalizeMembers()
{
}


TheEarth* TheEarth::theEarth = 0;
const irr::video::SColor TheEarth::baseColor;

void TheEarth::initialize()
{
    if (theEarth == 0)
    {
        theEarth = new TheEarth();
    }
}

void TheEarth::finalize()
{
    if (theEarth)
    {
        delete theEarth;
        theEarth = 0;
    }
}
    
    
TheEarth::TheEarth()
    : earthTexture(0),
      visualPart(0),
      newVisualPart(0),
      lastPosBox(),
      lastCenterPos(),
      lastCenterPosi(),
      xsize(0),
      ysize(0),
      height(0),
      density(0),
      hasDetail(0),
      isLoaded(0),
      hasDetailTex(0),
      maxHeight(0),
      minHeight(0xFFFF),
      tileMap()
{
    TheGame::getInstance()->getDevice()->getFileSystem()->grab();
    read();
}

TheEarth::~TheEarth()
{
    if (earthTexture)
    {
        earthTexture->drop();
        earthTexture = 0;
    }
    if (height)
    {
        delete [] height;
        height = 0;
    }
    if (density)
    {
        density->drop();
        density = 0;
    }
    if (hasDetail)
    {
        delete [] hasDetail;
        hasDetail = 0;
    }
    if (hasDetailTex)
    {
        delete [] hasDetailTex;
        hasDetailTex = 0;
    }
    
    for (tileMap_t::iterator it = tileMap.begin(); it != tileMap.end(); it++)
    {
        delete it->second;
    }
    tileMap.clear();
    TheGame::getInstance()->getDevice()->getFileSystem()->drop();
}

void TheEarth::run()
{
}

unsigned short TheEarth::getTileHeight(unsigned int x, unsigned int y)
{
    const unsigned int tileX = x / TILE_POINTS_NUM;
    const unsigned int tileY = y / TILE_POINTS_NUM;
    const unsigned int inX = x % TILE_POINTS_NUM;
    const unsigned int inY = y % TILE_POINTS_NUM;

    if (tileX < xsize && tileY < ysize)
    {
        Tile* tile;
        if (getIsLoaded(tileX, tileY))
        {
            tile = tileMap[tileX + (xsize*tileY)];
            tile->setInUse();
        }
        else
        {
            tile = new Tile(tileX, tileY,
                getEarthTexture(tileX, tileY),
                getEarthTexture(tileX+1, tileY),
                getEarthTexture(tileX, tileY+1),
                getEarthTexture(tileX+1, tileY+1));
            tileMap[tileX + (xsize*tileY)] = tile;
        }
        return tile->getHeight(inX, inY);
    }
    else
    {
        return 0;
    }
}

const irr::video::SColor& TheEarth::getTileTexture(unsigned int x, unsigned int y)
{
    const unsigned int tileX = x / TILE_POINTS_NUM;
    const unsigned int tileY = y / TILE_POINTS_NUM;
    const unsigned int inX = x % TILE_POINTS_NUM;
    const unsigned int inY = y % TILE_POINTS_NUM;

    if (tileX < xsize && tileY < ysize)
    {
        Tile* tile;
        if (getIsLoaded(tileX, tileY))
        {
            tile = tileMap[tileX + (xsize*tileY)];
            tile->setInUse();
        }
        else
        {
            tile = new Tile(tileX, tileY,
                getEarthTexture(tileX, tileY),
                getEarthTexture(tileX+1, tileY),
                getEarthTexture(tileX, tileY+1),
                getEarthTexture(tileX+1, tileY+1));
            tileMap[tileX + (xsize*tileY)] = tile;
        }
        return tile->getColor(inX, inY);
    }
    else
    {
        return baseColor;
    }
}

unsigned short TheEarth::getEarthHeight(unsigned int x, unsigned int y) const
{
    if (height && x < xsize && y < ysize)
    {
        return height[x + (xsize*y)];
    }
    else
    {
        return 0;
    }
}

bool TheEarth::setEarthHeight(unsigned int x, unsigned int y, unsigned short val)
{
    if (height && x < xsize && y < ysize)
    {
        height[x + (xsize*y)] = val;
        if (val < minHeight) minHeight = val;
        if (val > maxHeight) maxHeight = val;
        return true;
    }
    else
    {
        return false;
    }
}

bool TheEarth::getHasDetail(unsigned int x, unsigned int y) const
{
    unsigned int pos = (x + (xsize*y));
    if (hasDetail && x < xsize && y < ysize)
    {
        return (hasDetail[pos/8] & (0x1 << (pos%8))) == (0x1 << (pos%8));
    }
    else
    {
        return false;
    }
}

bool TheEarth::setHasDetail(unsigned int x, unsigned int y, bool val)
{
    unsigned int pos = (x + (xsize*y));
    if (hasDetail && x < xsize && y < ysize)
    {
        if (val)
        {
            hasDetail[pos/8] |= (0x1 << (pos%8));
        }
        else
        {
            hasDetail[pos/8] &= ~(0x1 << (pos%8));
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool TheEarth::getIsLoaded(unsigned int x, unsigned int y) const
{
    unsigned int pos = (x + (xsize*y));
    if (isLoaded && x < xsize && y < ysize)
    {
        return (isLoaded[pos/8] & (0x1 << (pos%8))) == (0x1 << (pos%8));
    }
    else
    {
        return false;
    }
}

bool TheEarth::setIsLoaded(unsigned int x, unsigned int y, bool val)
{
    unsigned int pos = (x + (xsize*y));
    if (isLoaded && x < xsize && y < ysize)
    {
        if (val)
        {
            isLoaded[pos/8] |= (0x1 << (pos%8));
        }
        else
        {
            isLoaded[pos/8] &= ~(0x1 << (pos%8));
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool TheEarth::getHasDetailTex(unsigned int x, unsigned int y) const
{
    unsigned int pos = (x + (xsize*y));
    if (hasDetailTex && x < xsize && y < ysize)
    {
        return (hasDetailTex[pos/8] & (0x1 << (pos%8))) == (0x1 << (pos%8));
    }
    else
    {
        return false;
    }
}

bool TheEarth::setHasDetailTex(unsigned int x, unsigned int y, bool val)
{
    unsigned int pos = (x + (xsize*y));
    if (hasDetailTex && x < xsize && y < ysize)
    {
        if (val)
        {
            hasDetailTex[pos/8] |= (0x1 << (pos%8));
        }
        else
        {
            hasDetailTex[pos/8] &= ~(0x1 << (pos%8));
        }
        return true;
    }
    else
    {
        return false;
    }
}

irr::video::SColor TheEarth::getEarthDensity(unsigned int x, unsigned int y) const
{
    if (density && xsize > 0 && ysize > 0)
    {
        if (x >= xsize) x = xsize - 1;
        if (y >= ysize) y = ysize - 1;
        return density->getPixel(x, y);
    }
    else
    {
        return baseColor;
    }
}

bool TheEarth::setEarthDensity(unsigned int x, unsigned int y, const irr::video::SColor& val)
{
    if (density && xsize > 0 && ysize > 0)
    {
        if (x >= xsize) x = xsize - 1;
        if (y >= ysize) y = ysize - 1;
        density->setPixel(x, y, val);
        return true;
    }
    else
    {
        return false;
    }
}

irr::video::SColor TheEarth::getEarthTexture(unsigned int x, unsigned int y) const
{
    if (density && xsize > 0 && ysize > 0)
    {
        if (x >= xsize) x = xsize - 1;
        if (y >= ysize) y = ysize - 1;
        return density->getPixel(x, y);
    }
    else
    {
        return baseColor;
    }
}


bool TheEarth::read()
{
    bool ret = true;
    ret &= readHeight();
    ret &= readHasDetail();
    ret &= readHasDetailTex();
    ret &= readDensity();
    ret &= readEarthTexture();
    return ret;
}

bool TheEarth::readHeight()
{
    int rc = 0;
    FILE* f;
    errno_t error = fopen_s(&f, "earthdata/earth_data_height.dat", "rb");
    if (error)
    {
        printf("unable to open file for read earthdata/earth_data_height.dat\n");
        return false;
    }

    rc = fscanf_s(f, "%u\n%u\n", &xsize, &ysize);
    if (rc < 2)
    {
        printf("unable to read xsize, ysize\n");
        fclose(f);
        return false;
    }

    if (height) delete [] height;

    height = new unsigned short[xsize*ysize];
    memset(height, 0, (xsize*ysize) * sizeof(unsigned short));

    unsigned int twsize = xsize * ysize;// * sizeof(unsigned short);
    unsigned int i = twsize;
    while (i)
    {
        size_t wsize = 1024;
        if (i < wsize) wsize = (size_t)i;
        size_t ret = fread(height+(twsize-i), sizeof(unsigned short), wsize, f);
        if (ret != wsize)
        {
             printf("unable to read offset %u / %u\n", twsize-i, twsize);
             fclose(f);
             return false;
        }
        i -= wsize;
    }

    maxHeight = 0;
    minHeight = 0xffff;

    for (unsigned int j = 0; j < ysize; j++)
    {
        for (unsigned int i = 0; i < xsize; i++)
        {
            unsigned short val = getEarthHeight(i, j);
            if (val > maxHeight) maxHeight = val;
            if (val < minHeight) minHeight = val;
        }
    }
    if (minHeight > maxHeight) minHeight = maxHeight;

    fclose(f);
    printf("%ux%u map read height ok, min %hu, max %hu, diff %hu, step %hu\n",
        xsize, ysize, minHeight, maxHeight, maxHeight - minHeight, (maxHeight-minHeight)/255);
    return true;
}

bool TheEarth::readHasDetail()
{
    int rc = 0;
    FILE* f;
    errno_t error = fopen_s(&f, "earthdata/earth_data_hasDetail.dat", "rb");
    if (error)
    {
        printf("unable to open file for read earthdata/earth_data_hasDetail.dat\n");
        return false;
    }

    rc = fscanf_s(f, "%u\n%u\n", &xsize, &ysize);
    if (rc < 2)
    {
        printf("unable to read xsize, ysize\n");
        fclose(f);
        return false;
    }

    if (hasDetail) delete [] hasDetail;

    hasDetail = new unsigned char[(xsize*ysize)/8+1];
    memset(hasDetail, 0, (xsize*ysize)/8+1);

    unsigned int twsize = (xsize*ysize)/8+1;
    unsigned int i = twsize;
    while (i)
    {
        size_t wsize = 1024;
        if (i < wsize) wsize = (size_t)i;
        size_t ret = fread(hasDetail+(twsize-i), sizeof(unsigned char), wsize, f);
        if (ret != wsize)
        {
             printf("unable to read road data offset %u / %u\n", twsize-i, twsize);
             fclose(f);
             return false;
        }
        i -= wsize;
    }

    fclose(f);
    printf("%ux%u map read hasDetail ok\n", xsize, ysize);
    return true;
}

bool TheEarth::readHasDetailTex()
{
    int rc = 0;
    FILE* f;
    errno_t error = fopen_s(&f, "earthdata/earth_data_hasDetailTex.dat", "rb");
    if (error)
    {
        printf("unable to open file for read earthdata/earth_data_hasDetailTex.dat\n");
        return false;
    }

    rc = fscanf_s(f, "%u\n%u\n", &xsize, &ysize);
    if (rc < 2)
    {
        printf("unable to read xsize, ysize\n");
        fclose(f);
        return false;
    }

    if (hasDetailTex) delete [] hasDetailTex;

    hasDetailTex = new unsigned char[(xsize*ysize)/8+1];
    memset(hasDetailTex, 0, (xsize*ysize)/8+1);

    unsigned int twsize = (xsize*ysize)/8+1;
    unsigned int i = twsize;
    while (i)
    {
        size_t wsize = 1024;
        if (i < wsize) wsize = (size_t)i;
        size_t ret = fread(hasDetailTex+(twsize-i), sizeof(unsigned char), wsize, f);
        if (ret != wsize)
        {
             printf("unable to read road data offset %u / %u\n", twsize-i, twsize);
             fclose(f);
             return false;
        }
        i -= wsize;
    }

    fclose(f);
    printf("%ux%u map read hasDetailTex ok\n", xsize, ysize);
    return true;
}

bool TheEarth::readDensity()
{

    if (density) density->drop();
    density = TheGame::getInstance()->getDriver()->createImageFromFile("earthdata/earth_data_density.png");
    printf("%ux%u map read density ok\n", density->getDimension().Width, density->getDimension().Height);
    return true;
}

bool TheEarth::readEarthTexture()
{

    if (earthTexture) earthTexture->drop();
    earthTexture = TheGame::getInstance()->getDriver()->createImageFromFile("earthdata/earth_data_texture.png");
    printf("%ux%u map read texture ok\n", earthTexture->getDimension().Width, earthTexture->getDimension().Height);
    return true;
}


bool TheEarth::write()
{
    bool ret = true;
    ret &= writeHeight();
    ret &= writeHasDetail();
    ret &= writeHasDetailTex();
    return ret;
}

bool TheEarth::writeHeight()
{
    if (height==0) return false;
    
    int rc = 0;
    FILE* f;
    errno_t error = fopen_s(&f, "earthdata/earth_data_height.dat", "wb");
    if (error) return false;

    rc = fprintf(f, "%u\n%u\n", xsize, ysize);
    if (rc <= 0)
    {
        printf("unable to write sizes\n");
        fclose(f);
        return false;
    }

    unsigned int twsize = xsize * ysize;// * sizeof(unsigned short);
    unsigned int i = twsize;
    while (i)
    {
        size_t wsize = 1024;
        if (i < wsize) wsize = (size_t)i;
        size_t ret = fwrite(height+(twsize-i), sizeof(unsigned short), wsize, f);
        if (ret != wsize)
        {
             printf("unable to write offset %u / %u\n", twsize-i, twsize);
             fclose(f);
             return false;
        }
        i -= wsize;
    }
    fclose(f);
    printf("%ux%u map write height ok, min %hu, max %hu, diff %hu, step %hu\n",
        xsize, ysize, minHeight, maxHeight, maxHeight - minHeight, (maxHeight-minHeight)/255);
    return true;
}

bool TheEarth::writeHasDetail()
{
    if (hasDetail==0) return false;
    
    int rc = 0;
    FILE* f;
    errno_t error = fopen_s(&f, "earthdata/earth_data_hasDetail.dat", "wb");
    if (error) return false;

    rc = fprintf(f, "%u\n%u\n", xsize, ysize);
    if (rc <= 0)
    {
        printf("unable to write sizes\n");
        fclose(f);
        return false;
    }

    unsigned int twsize = (xsize*ysize)/8+1;
    unsigned int i = twsize;
    while (i)
    {
        size_t wsize = 1024;
        if (i < wsize) wsize = (size_t)i;
        size_t ret = fwrite(hasDetail+(twsize-i), sizeof(unsigned char), wsize, f);
        if (ret != wsize)
        {
             printf("unable to write road data offset %u / %u\n", twsize-i, twsize);
             fclose(f);
             return false;
        }
        i -= wsize;
    }

    fclose(f);
    printf("%ux%u map write hasDetail ok\n", xsize, ysize);
    return true;
}

bool TheEarth::writeHasDetailTex()
{
    if (hasDetailTex==0) return false;
    
    int rc = 0;
    FILE* f;
    errno_t error = fopen_s(&f, "earthdata/earth_data_hasDetailTex.dat", "wb");
    if (error) return false;

    rc = fprintf(f, "%u\n%u\n", xsize, ysize);
    if (rc <= 0)
    {
        printf("unable to write sizes\n");
        fclose(f);
        return false;
    }

    unsigned int twsize = (xsize*ysize)/8+1;
    unsigned int i = twsize;
    while (i)
    {
        size_t wsize = 1024;
        if (i < wsize) wsize = (size_t)i;
        size_t ret = fwrite(hasDetailTex+(twsize-i), sizeof(unsigned char), wsize, f);
        if (ret != wsize)
        {
             printf("unable to write road data offset %u / %u\n", twsize-i, twsize);
             fclose(f);
             return false;
        }
        i -= wsize;
    }

    fclose(f);
    printf("%ux%u map write hasDetailTex ok\n", xsize, ysize);
    return true;
}

bool TheEarth::writeToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const
{
    irr::video::IImageWriter* writer = 0;
    irr::io::path path = "earth_data.png";

    for (unsigned int i = 0; i < driver->getImageWriterCount(); i++)
    {
        irr::video::IImageWriter* tmpwriter = driver->getImageWriter(i);
        if (tmpwriter->isAWriteableFileExtension(path))
        {
            writer = tmpwriter;
            break;
        }
    }

    if (writer == 0)
    {
        printf("There is no writer for [earth_data.png]\n");
        return false;
    }

    bool ret = true;
    ret &= writeHeightToPNG(device, driver);
    ret &= writeHasDetailToPNG(device, driver);
    // ret &= writeHasDetailTexToPNG(device, driver);

    ret &= writeDensityToPNG(device, driver);
    ret &= writeEarthTextureToPNG(device, driver);
    return ret;
}

bool TheEarth::writeHeightToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const
{
    if (height==0) return false;
    if (minHeight == maxHeight)
    {
        printf("min max equal\n");
        return false;
    }
    irr::video::IImage* image = driver->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<irr::u32>(xsize, ysize));
    irr::io::path path = "earthdata/earth_data_height.png";

    for (unsigned int i = 0; i < xsize; i++)
    {
        for (unsigned int j = 0; j < ysize; j++)
        {
            unsigned int color = (255*(unsigned int)(getEarthHeight(i, j)-minHeight))/(unsigned int)(maxHeight-minHeight);
            image->setPixel(i, j, irr::video::SColor(255, color, color, color));
        }
    }

    bool ret = driver->writeImageToFile(image, device->getFileSystem()->createAndWriteFile(path));
    image->drop();
    return ret;
}

bool TheEarth::writeHasDetailToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const
{
    if (hasDetail==0) return false;

    irr::video::IImage* image = driver->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<irr::u32>(xsize, ysize));
    irr::io::path path = "earthdata/earth_data_hasDetail.png";

    for (unsigned int i = 0; i < xsize; i++)
    {
        for (unsigned int j = 0; j < ysize; j++)
        {
            unsigned int color = getHasDetail(i, j) ? 255 : 0;
            image->setPixel(i, j, irr::video::SColor(255, color, color, color));
        }
    }

    bool ret = driver->writeImageToFile(image, device->getFileSystem()->createAndWriteFile(path));
    image->drop();
    return ret;
}


bool TheEarth::writeDensityToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const
{
    if (density==0) return false;

    irr::io::path path = "earthdata/earth_data_density.png";

    bool ret = driver->writeImageToFile(density, device->getFileSystem()->createAndWriteFile(path));
    return ret;
}

bool TheEarth::writeEarthTextureToPNG(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver) const
{
    if (earthTexture==0) return false;

    irr::io::path path = "earthdata/earth_data_texture.png";

    bool ret = driver->writeImageToFile(earthTexture, device->getFileSystem()->createAndWriteFile(path));
    return ret;
}

void TheEarth::createFirst(const irr::core::vector3df& pos, const irr::core::vector3df& dir)
{
    lastCenterPosi = irr::core::vector3di((int)pos.X, 0, (int)pos.Z);
    lastCenterPos = irr::core::vector3df((float)lastCenterPosi.X, (float)lastCenterPosi.Y, (float)lastCenterPosi.Z);
    lastPosBox = irr::core::aabbox3df(lastCenterPos.X-VISUAL_BOX_HSIZE_F, -1000.0f, lastCenterPos.Z-VISUAL_BOX_HSIZE_F,
        lastCenterPos.X+VISUAL_BOX_HSIZE_F, 10000.0f, lastCenterPos.Z+VISUAL_BOX_HSIZE_F);

    if (visualPart)
    {
        delete visualPart;
        visualPart = 0;
    }
    if (newVisualPart)
    {
        delete newVisualPart;
        newVisualPart = 0;
    }

    visualPart = new VisualMembers();
    visualPart->createMembers(lastCenterPosi, this);
    visualPart->loadMembers(this);
}

void TheEarth::update(const irr::core::vector3df& pos, const irr::core::vector3df& dir)
{
}
