
#include "VehicleType.h"
#include "Vehicle.h"
#include "VehicleTypeManager.h"
#include "ObjectPoolManager.h"
#include "stdafx.h"

Vehicle::Vehicle(const std::string& vehicleTypeName, const irr::core::vector3df& apos, const irr::core::vector3df& rotation)
    : vehicleType(0),
      matrix(),
      offsetObject(0),
      node(0),
      hkBody(0)
{
    vehicleType = VehicleTypeManager::getInstance()->getVehicleType(vehicleTypeName);

    offsetObject = ObjectPoolManager::getInstance()->getObject(vehicleType->objectName, apos);
    offsetObject->setUpdateCB(this);
    node = offsetObject->getNode();
    hkBody = offsetObject->getBody();

    matrix.setTranslation(node->getPosition());
    matrix.setRotationDegrees(rotation);
}

Vehicle::~Vehicle()
{
    vehicleType = 0;
    node = 0;
    hkBody = 0;

    ObjectPoolManager::getInstance()->putObject(offsetObject);
    offsetObject = 0;
}

void Vehicle::handleUpdatePos(bool phys)
{
}

void Vehicle::reset(const irr::core::vector3df& pos)
{
    irr::core::vector3df rot = matrix.getRotationDegrees();// - m_trafo.getRotationDegrees();
    dprintf(MY_DEBUG_NOTE, "reset car: orig rot: %f %f %f\n", rot.X, rot.Y, rot.Z);
    if (fabsf(rot.Z-180.f) < 90.f)
    {
        if (rot.Y < 90.f)
            rot.Y = 180.f - rot.Y;
        if (rot.Y > 270.f)
            rot.Y = 540.f - rot.Y;
    }
    rot.Z = rot.X = 0.f;
    dprintf(MY_DEBUG_NOTE, "reset car:  mod rot: %f %f %f\n", rot.X, rot.Y, rot.Z);
    matrix.setTranslation(pos);
    // vector3df(0.f, rot.Y, 0.f)
    matrix.setRotationDegrees(rot);
    hkBody->setLinearVelocity(hkVector4());
    updateToMatrix();
}

float Vehicle::getAngle()
{
    irr::core::vector3df rot = matrix.getRotationDegrees();
    //dprintf(printf("reset car: orig rot: %f %f %f\n", rot.X, rot.Y, rot.Z));
    if (fabsf(rot.Z-180.f) < 90.f)
    {
        if (rot.Y < 90.f)
            rot.Y = 180.f - rot.Y;
        if (rot.Y > 270.f)
            rot.Y = 540.f - rot.Y;
    }
    rot.Z = rot.X = 0.f;
    //dprintf(printf("reset car:  mod rot: %f %f %f\n", rot.X, rot.Y, rot.Z));
    return rot.Y;
}

void Vehicle::updateToMatrix()
{
    irr::core::vector3df pos = matrix.getTranslation();
    irr::core::quaternion quat(matrix);
    hkBody->setPositionAndRotation(hkVector4(pos.X, pos.Y, pos.Z), hkQuaternion(quat.X, quat.Y, quat.Z, quat.W));

    node->setPosition(pos);
    node->setRotation((matrix).getRotationDegrees());
}
