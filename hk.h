#ifndef HK_H
#define HK_H

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
//#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/hkpVehicle.h>

class hk
{
public:
    static void initialize();
    static void finalize();

public:
    static hkpWorld*        hkWorld;

private:
    static hkJobThreadPool* threadPool;
    static int              totalNumThreadsUsed;
    static hkJobQueue*      jobQueue;
};

#endif // HK_H