#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <irrlicht.h>
#include <vector>
#include "hk.h"
#include "OffsetObject.h"

#include <Physics/Vehicle/AeroDynamics/Default/hkpVehicleDefaultAerodynamics.h>
#include <Physics/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>
#include <Physics/Vehicle/Brake/Default/hkpVehicleDefaultBrake.h>
#include <Physics/Vehicle/Engine/Default/hkpVehicleDefaultEngine.h>
#include <Physics/Vehicle/VelocityDamper/Default/hkpVehicleDefaultVelocityDamper.h>
#include <Physics/Vehicle/Steering/Default/hkpVehicleDefaultSteering.h>
#include <Physics/Vehicle/Suspension/Default/hkpVehicleDefaultSuspension.h>
#include <Physics/Vehicle/Transmission/Default/hkpVehicleDefaultTransmission.h>
#include <Physics/Vehicle/WheelCollide/RayCast/hkpVehicleRayCastWheelCollide.h>
#include <Physics/Vehicle/TyreMarks/hkpTyremarksInfo.h>


class VehicleType;
class VehicleTypeTyre;


// -------------------------------------------------------
//                     VehicleTyre
// -------------------------------------------------------

class Vehicle;
class VehicleTyre
{
public:
    VehicleTyre(VehicleTypeTyre* p_vehicleTypeTyre, const irr::core::vector3df& apos, Vehicle* vehicle, unsigned int tyreNum);
    ~VehicleTyre();

private:
    Vehicle*                vehicle;
    unsigned int            tyreNum;
    irr::core::matrix4      localMatrix;
    irr::scene::ISceneNode* node;
    VehicleTypeTyre*        tyreType;
    OffsetObject*           offsetObject;
    hkpRigidBody*           hitBody;
    float                   width;
    float                   radius;

    friend class Vehicle;
    friend class FrictionMapVehicleRaycastWheelCollide;
};
typedef std::vector<VehicleTyre*> tyreVector_t;


// -------------------------------------------------------
//                       Vehicle
// -------------------------------------------------------

class Vehicle : public OffsetObjectUpdateCB
{
public:
    Vehicle(const std::string& vehicleTypeName, const irr::core::vector3df& apos, const irr::core::vector3df& rotation);
    ~Vehicle();

    void reset(const irr::core::vector3df& pos);
    float getAngle();
    int getGear() const {return hkVehicle->m_currentGear+1;}
    float getSpeed() const {return hkVehicle->calcKMPH();}

    void setSteer(float value);
    void setTorque(float value);
    void setHandbrake(float value);

private:
    virtual void handleUpdatePos(bool phys);
    void updateToMatrix();

private:
    VehicleType*                vehicleType;

    irr::core::matrix4          matrix;
    irr::scene::ISceneNode*     node;
    hkpRigidBody*               hkBody;
    OffsetObject*               offsetObject;
    tyreVector_t                tyres;
    hkpVehicleInstance*         hkVehicle;

    friend class FrictionMapVehicleRaycastWheelCollide;
    friend class VehicleTyre;
};

#endif // VEHICLE_H
