
#include "TheGame.h"
#include "Tile.h"

const irr::video::SColor Tile::baseColor;

Tile::Tile(unsigned int posx, unsigned int posy,
           const irr::video::SColor& color00,
           const irr::video::SColor& color10,
           const irr::video::SColor& color01,
           const irr::video::SColor& color11,
           const irr::video::SColor& density,
           bool autoRead)
    : catx(posx/CATDIV), caty(posy/CATDIV),
      posx(posx), posy(posy),
      height(new unsigned short[TILE_POINTS_NUM*TILE_POINTS_NUM]),
      inUse(true),
      colors(new irr::video::SColor[TILE_POINTS_NUM*TILE_POINTS_NUM]),
      fineColors(new irr::video::SColor[TILE_FINE_POINTS_NUM*TILE_FINE_POINTS_NUM]),
      fineDensity(new irr::video::SColor[TILE_FINE_POINTS_NUM*TILE_FINE_POINTS_NUM])
{
    memset(height, 0, (TILE_POINTS_NUM*TILE_POINTS_NUM)*sizeof(unsigned short));
    if (autoRead)
    {
        read();
    }
    for (unsigned int x = 0; x < TILE_POINTS_NUM; x++)
    {
        const unsigned int cdiv = (TILE_POINTS_NUM + 1);
        const unsigned int cx0 = cdiv - x;
        const unsigned int cx1 = /*cdiv -*/ x;
        const irr::video::SColor color0(255,
              (color00.getRed()*cx0 +   color10.getRed()*cx1) / cdiv,
            (color00.getGreen()*cx0 + color10.getGreen()*cx1) / cdiv,
             (color00.getBlue()*cx0 +  color10.getBlue()*cx1) / cdiv);
        const irr::video::SColor color1(255,
              (color01.getRed()*cx0 +   color11.getRed()*cx1) / cdiv,
            (color01.getGreen()*cx0 + color11.getGreen()*cx1) / cdiv,
             (color01.getBlue()*cx0 +  color11.getBlue()*cx1) / cdiv);
        for (unsigned int y = 0; y < TILE_POINTS_NUM; y++)
        {
            const unsigned int cy0 = cdiv - y;
            const unsigned int cy1 = y;

            const unsigned int red =     (color0.getRed()*cy0 +   color1.getRed()*cy1) / cdiv;
            const unsigned int green = (color0.getGreen()*cy0 + color1.getGreen()*cy1) / cdiv;
            const unsigned int blue =   (color0.getBlue()*cy0 +  color1.getBlue()*cy1) / cdiv;
            //printf("%u %u %u\n", red, green, blue);
            //printf("%u ", red);
            colors[x + (TILE_POINTS_NUM*y)].set(255, red, green, blue);
        }
        //printf("\n\n");
    }

    for (unsigned int x = 0; x < TILE_POINTS_NUM - 1; x++)
    {
        for (unsigned int y = 0; y < TILE_POINTS_NUM - 1; y++)
        {
            const irr::video::SColor& fcolor00 = getColor(x, y);
            const irr::video::SColor& fcolor10 = getColor(x + 1, y);
            const irr::video::SColor& fcolor01 = getColor(x, y + 1);
            const irr::video::SColor& fcolor11 = getColor(x + 1, y + 1);
            
            for (unsigned int fx = 0; fx < TILE_FINE_RATE; fx++)
            {
                const unsigned int cdiv = (TILE_FINE_RATE + 1);
                const unsigned int cx0 = cdiv - fx;
                const unsigned int cx1 = /*cdiv -*/ fx;
                const irr::video::SColor color0(255,
                      (fcolor00.getRed()*cx0 +   fcolor10.getRed()*cx1) / cdiv,
                    (fcolor00.getGreen()*cx0 + fcolor10.getGreen()*cx1) / cdiv,
                     (fcolor00.getBlue()*cx0 +  fcolor10.getBlue()*cx1) / cdiv);
                const irr::video::SColor color1(255,
                      (fcolor01.getRed()*cx0 +   fcolor11.getRed()*cx1) / cdiv,
                    (fcolor01.getGreen()*cx0 + fcolor11.getGreen()*cx1) / cdiv,
                     (fcolor01.getBlue()*cx0 +  fcolor11.getBlue()*cx1) / cdiv);
                for (unsigned int fy = 0; fy < TILE_POINTS_NUM; fy++)
                {
                    const unsigned int cy0 = cdiv - fy;
                    const unsigned int cy1 = fy;
    
                    const unsigned int red =     (color0.getRed()*cy0 +   color1.getRed()*cy1) / cdiv;
                    const unsigned int green = (color0.getGreen()*cy0 + color1.getGreen()*cy1) / cdiv;
                    const unsigned int blue =   (color0.getBlue()*cy0 +  color1.getBlue()*cy1) / cdiv;
                    //printf("%u %u %u\n", red, green, blue);
                    //printf("%u ", red);
                    fineColors[((x*TILE_FINE_RATE+fx)) + (((y*TILE_FINE_RATE)+fy)*TILE_FINE_POINTS_NUM)].set(255, red, green, blue);
                }
                //printf("\n\n");
            }
        }
        //printf("\n\n");
    }
    
    for (unsigned int x = 0; x < TILE_FINE_POINTS_NUM; x++)
    {
        for (unsigned int y = 0; y < TILE_FINE_POINTS_NUM; y++)
        {
            fineDensity[x + (TILE_FINE_POINTS_NUM*y)] = density;
        }
    }
    printf("tile pos(%u): %u, %u\n", posx + (6750*posy), posx, posy);
    
    printf("%u %u %u \n", color00.getRed(), color00.getGreen(), color00.getBlue());
    printf("%u %u %u \n", color10.getRed(), color10.getGreen(), color10.getBlue());
    printf("%u %u %u \n", color01.getRed(), color01.getGreen(), color01.getBlue());
    printf("%u %u %u \n", color11.getRed(), color11.getGreen(), color11.getBlue());
    printf("\n");
    
    //assert(0);
}

Tile::~Tile()
{
    if (height)
    {
        delete [] height;
        height = 0;
    }
    if (colors)
    {
        delete [] colors;
        colors = 0;
    }
    if (fineColors)
    {
        delete [] fineColors;
        fineColors = 0;
    }
}

bool Tile::setHeight(unsigned int x, unsigned int y, unsigned short val)
{
    if (height && x < TILE_POINTS_NUM && y < TILE_POINTS_NUM)
    {
        height[x + (TILE_POINTS_NUM*y)] = val;
        //if (val < minHeight) minHeight = val;
        //if (val > maxHeight) maxHeight = val;
        return true;
    }
    else
    {
        return false;
    }
}

bool Tile::read()
{
    int rc = 0;
    char filename[256];
    char zipfilename[256];
    char dirname[256];
    sprintf_s(dirname, "data/earthdata/tiles/%d_%d", catx, caty);
    sprintf_s(zipfilename, "data/earthdata/tiles/%d_%d.zip", catx, caty);
    sprintf_s(filename, "%s/%d_%d.dat", dirname, posx, posy);

    TheGame::getInstance()->getDevice()->getFileSystem()->addFileArchive(zipfilename, false, false, irr::io::EFAT_ZIP);
    irr::io::IReadFile* file = TheGame::getInstance()->getDevice()->getFileSystem()->createAndOpenFile(filename);
    
    if (!file)
    {
        printf("%s tile file not found\n", filename);
        assert(0);
        return false;
    }

    if (!height)    
    {
        height = new unsigned short[TILE_POINTS_NUM*TILE_POINTS_NUM];
        memset(height, 0, (TILE_POINTS_NUM*TILE_POINTS_NUM)*sizeof(unsigned short));
    }

    file->seek(6);    
    rc = file->read(height, (TILE_POINTS_NUM*TILE_POINTS_NUM)*sizeof(unsigned short));
    file->drop();


    return true;
}
