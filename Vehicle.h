#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <irrlicht.h>
#include <vector>
#include "hk.h"
#include "OffsetObject.h"

class VehicleType;
//typedef std::vector<OffsetObject*> tyreOffsetObjectsVector_t;

class Vehicle : public OffsetObjectUpdateCB
{
public:
    Vehicle(const std::string& vehicleTypeName, const irr::core::vector3df& apos, const irr::core::vector3df& rotation);
    ~Vehicle();

    void reset(const irr::core::vector3df& pos);
    float getAngle();

private:
    virtual void handleUpdatePos(bool phys);
    void updateToMatrix();

private:
    VehicleType*                vehicleType;

    irr::core::matrix4          matrix;
    OffsetObject*               offsetObject;
    irr::scene::ISceneNode*     node;
    hkpRigidBody*               hkBody;
};

#endif // VEHICLE_H
