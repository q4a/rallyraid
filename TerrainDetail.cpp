
#include "TerrainDetail.h"
#include "Terrain_defs.h"
#include "TheGame.h"
#include "OffsetObject.h"
#include "OffsetManager.h"
#include "TheEarth.h"
#include <math.h>
#include "Shaders.h"
#include "Settings.h"

#include <string.h>
#include <assert.h>

class HeightFieldHelperDetail : public hkpSampledHeightFieldShape
{
public:
    HeightFieldHelperDetail(const hkpSampledHeightFieldBaseCinfo& ci, TerrainDetail* td)
        : hkpSampledHeightFieldShape(ci),
          td(td)
    {
    }
    
    HK_FORCE_INLINE hkReal getHeightAtImpl(int x, int z) const
    {
        // Call in the setvisible state, when create
        //printf("gh ");
        float h = td->get(x, z);//fineHeights[(x+TILE_POINTS_NUM) + (TILE_DETAIL_POINTS_NUM*(z+TILE_POINTS_NUM))];
        //earth->getTileHeight((unsigned int)abs(offsetX+x), (unsigned int)abs(offsetY+z));
        //printf("x: %d, y: %d, offsetX: %d, offsetY: %d, h: %hu\n", x, z, offsetX, offsetY, h);
        //assert(0);
        return (hkReal)h;
    }
    
    /// this should return true if the two triangles share the edge p00-p11
    ///      otherwise it should return false if the triangles shere the edge p01-p10
    HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
    {
        return true;
    }
    
    virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
    {
        hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
    }
private:
    TerrainDetail* td;
};


TerrainDetail::TerrainDetail(const irr::core::vector3di& posi, TheEarth* earth)
    : Terrain("detail"),
      fineHeights(new float[(TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*(TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))])
{
    offsetX = posi.X / TILE_DETAIL_SCALE;
    offsetY = posi.Z / TILE_DETAIL_SCALE;

    terrain = new irr::scene::TerrainSceneNode(0, 
        TheGame::getInstance()->getSmgr(),
        TheGame::getInstance()->getDevice()->getFileSystem(),
        -1,
        4,
        irr::scene::ETPS_17,
        irr::core::vector3df((float)posi.X, 0.0f, (float)posi.Z)/*-OffsetManager::getInstance()->getOffset()*/,
        irr::core::vector3df(),
        irr::core::vector3df(TILE_DETAIL_SCALE_F, 1.0f, TILE_DETAIL_SCALE_F));
    terrain->setVisible(visible);
    offsetObject = new OffsetObject(terrain);

    // generate fineHeights
    {
        memset(fineHeights, 0, (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*(TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*sizeof(float));
        
        // first fill the base points + 2 because of the borders for the interpolation.
        int offsetXOrig = posi.X / TILE_SCALE - 1;
        int offsetYOrig = posi.Z / TILE_SCALE - 1;
        bool nonZero = false;
        
        for (int y = 0, ffy = 0; y < TILE_POINTS_NUM + 3; y++, ffy+=(TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE)
        {
            for (int x = 0, ffx = 0; x < TILE_POINTS_NUM + 3; x++, ffx+=TILE_DETAIL_RATE)
            {
                unsigned short h = earth->getTileHeight((unsigned int)abs(offsetXOrig+x), (unsigned int)abs(offsetYOrig+y));
                fineHeights[ffx+ffy] = (float)h;
                nonZero |= (h > 0);
            }
        }
        
        // if we have non-zero height, then do interpolate
        if (nonZero)
        {
#if 1
            // first interpolate rows
            for (int y = TILE_DETAIL_RATE, fy = (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE;
                 y < TILE_DETAIL_POINTS_NUM+(2*TILE_DETAIL_RATE);
                 y+=TILE_DETAIL_RATE, fy+=(TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE)
            {
                int c = 0;
                const int cm1 = -TILE_DETAIL_RATE;
                const int cp1 = TILE_DETAIL_RATE;
                const int cp2 = 2*TILE_DETAIL_RATE;
                float hcm1 = 0;
                float hc = 0;
                float hcp1 = 0;
                float hcp2 = 0;
                float b0, b1, b2, b3;
                for(int i = TILE_DETAIL_RATE; i < TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE); i++)
                {
                    if (i%TILE_DETAIL_RATE==0)
                    {
                        c+=TILE_DETAIL_RATE;
                        assert(c==i);
                        hcm1 = get(c+cm1+fy);
                        hc   = get(c+fy);
                        hcp1 = get(c+cp1+fy);
                        hcp2 = get(c+cp2+fy);
                        b0   = (hcm1 + 4.0f*hc + hcp1) / 6.0f;
                        //printf("\tb0 = (yim1 + 4*yi + yip1) / 6 = %d\n", b0);
                        b1   = (-hcm1 + hcp1) / 2.0f;
                        //printf("\tb1 = (-yim1 + yip1) / 2 = %d\n", b1);
                        b2   = (hcm1 - 2.0f*hc + hcp1) / 2.0f;
                        //printf("\tb2 = (yim1 - 2*yi + yip1) / 2 = %d\n", b2);
                        b3   = (-hcm1 + 3.0f*hc - 3.0f*hcp1 + hcp2) / 6.0f;
                    }
                    float t = (float)(i - c) / (TILE_DETAIL_RATE_F);
                    if (y == TILE_DETAIL_RATE || y == TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE) ||
                        i == TILE_DETAIL_RATE /*|| i == TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE)*/)
                    {
                        /*fineHeights[i+fy] =*/set(i+fy, (1.0f-t)*hc + t*hcp1);
                    }
                    else
                    {
                        /*fineHeights[i+fy] =*/set(i+fy, ((((((b3*t)) + b2) * t) + b1) * t) + b0);
                    }
                }
            }
            // second interpolate column
            for (int x = TILE_DETAIL_RATE; x < TILE_DETAIL_POINTS_NUM+(2*TILE_DETAIL_RATE); x+=TILE_DETAIL_RATE)
            {
                int c = 0;
                //int fc = 0;
                const int cm1 = -((TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE);
                const int cp1 = (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE;
                const int cp2 = (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*2*TILE_DETAIL_RATE;
                float hcm1 = 0;
                float hc = 0;
                float hcp1 = 0;
                float hcp2 = 0;
                float b0, b1, b2, b3;
                //for(int i = TILE_DETAIL_RATE, fi; i < TILE_DETAIL_POINTS_NUM+TILE_DETAIL_RATE; i++)
                for (int y = TILE_DETAIL_RATE, fy = (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE;
                     y < TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE);
                     y++, fy+=TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))
                {
                    if (y%TILE_DETAIL_RATE==0)
                    {
                        c+=TILE_DETAIL_RATE;
                        //fc+=TILE_DETAIL_POINTS_NUM+(2*TILE_DETAIL_RATE);
                        assert(c==y);
                        //assert(fc==fy);
                        hcm1 = get(x+cm1+fy);
                        hc   = get(x+fy);
                        hcp1 = get(x+cp1+fy);
                        hcp2 = get(x+cp2+fy);
                        b0   = (hcm1 + 4.0f*hc + hcp1) / 6.0f;
                        //printf("\tb0 = (yim1 + 4*yi + yip1) / 6 = %d\n", b0);
                        b1   = (-hcm1 + hcp1) / 2.0f;
                        //printf("\tb1 = (-yim1 + yip1) / 2 = %d\n", b1);
                        b2   = (hcm1 - 2.0f*hc + hcp1) / 2.0f;
                        //printf("\tb2 = (yim1 - 2*yi + yip1) / 2 = %d\n", b2);
                        b3   = (-hcm1 + 3.0f*hc - 3.0f*hcp1 + hcp2) / 6.0f;
                    }
                    float t = (float)(y - c) / (TILE_DETAIL_RATE_F);
                    if (x == TILE_DETAIL_RATE || x == TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE) ||
                        y == TILE_DETAIL_RATE /*|| y == TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE)*/)
                    {
                        /*fineHeights[x+fy] =*/ set(x+fy, (1.0f-t)*hc + t*hcp1);
                    }
                    else
                    {
                        /*fineHeights[x+fy] =*/ set(x+fy, ((((((b3*t)) + b2) * t) + b1) * t) + b0);
                    }
                }
            }
#endif // 0 v 1
            // fill other with liner interpolation
            int lty = 0;
            int flty = 0;
            int ltx = 0;
            int ty = 0;
            int tx = 0;
            int typ1 = 0;
            int txp1 = 0;
            int ltyp1 = 0;
            int fltyp1 = 0;
            int ltxp1 = 0;
            float hy;
            float hyp1;
            float hx;
            float hxp1;
            for (int y = TILE_DETAIL_RATE, fy = (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE;
                 y < TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE);
                 y++, fy+=TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))
            {
                typ1 = y%TILE_DETAIL_RATE;
                ty = TILE_DETAIL_RATE - typ1;
                if (typ1 != 0)
                {
                    for(int x = TILE_DETAIL_RATE; x < TILE_DETAIL_POINTS_NUM+(1*TILE_DETAIL_RATE); x++)
                    {
                        float h = get(x+fy);
                        txp1 = x%TILE_DETAIL_RATE;
                        tx = TILE_DETAIL_RATE - txp1;
                        if (txp1 != 0)
                        {
                            assert(h==0);
                            hy = get(x+flty) /* 100*/;
                            hyp1 = get(x+fltyp1) /* 100*/;
                            set(x+fy, (hy*(float)ty+hyp1*(float)typ1+hx*(float)tx+hxp1*(float)txp1)/(TILE_DETAIL_RATE_F*2.0f) /*- 2.5f*/);
                        }
                        else
                        {
                            ltx = x;
                            ltxp1 = x + TILE_DETAIL_RATE;
                            hx = get(ltx+fy) /* 100*/;
                            hxp1 = get(ltxp1+fy) /* 100*/;
                        }
                    }
                }
                else
                {
                    lty = y;
                    flty = fy;
                    ltyp1 = y + TILE_DETAIL_RATE;
                    fltyp1 = fy + (TILE_DETAIL_POINTS_NUM+(3*TILE_DETAIL_RATE))*TILE_DETAIL_RATE;
                }
            }
        }
    }
    hk::lock();
    hkpSampledHeightFieldBaseCinfo ci;
    ci.m_xRes = TILE_DETAIL_POINTS_NUM + 1;
    ci.m_zRes = TILE_DETAIL_POINTS_NUM + 1;
    ci.m_scale.set(TILE_DETAIL_SCALE_F, 1.0f, TILE_DETAIL_SCALE_F);

    hkShape = new HeightFieldHelperDetail(ci, this);
    hk::unlock();
    postConstruct();
}

TerrainDetail::~TerrainDetail()
{
    if (fineHeights)
    {
        delete [] fineHeights;
        fineHeights = 0;
    }
}

void TerrainDetail::load(TheEarth* earth)
{
    terrain->loadHeightMap(this, offsetX, offsetY, TILE_DETAIL_POINTS_NUM+1/*, image*/);
    if (!image)
    {
        image = TheGame::getInstance()->getDriver()->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2du(TILE_FINE_POINTS_NUM, TILE_FINE_POINTS_NUM));
        int offsetXFine = offsetX * TILE_DETAIL_FINE_RATE;
        int offsetYFine = offsetY * TILE_DETAIL_FINE_RATE;
        printf("===============================\n%d %d\n===============================\n", offsetXFine, offsetYFine);
        for (int x = 0; x < TILE_FINE_POINTS_NUM; x++)
        {
            for (int z = 0; z < TILE_FINE_POINTS_NUM; z++)
            {
                image->setPixel(TILE_FINE_POINTS_NUM - x - 1, z, earth->getTileFineTexture((unsigned int)abs(offsetXFine+x), (unsigned int)abs(offsetYFine+z)));
            }
        }
    }
    postLoad();
}

