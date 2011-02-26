#include "VehicleManager.h"

VehicleManager* VehicleManager::vehicleManager = 0;

void VehicleManager::initialize()
{
    if (!vehicleManager)
    {
        vehicleManager = new VehicleManager();
    }
}

void VehicleManager::finalize()
{
    if (vehicleManager)
    {
        delete vehicleManager;
        vehicleManager = 0;
    }
}
    

VehicleManager::VehicleManager()
    : vehicleSet()
{
}

VehicleManager::~VehicleManager()
{
    // TODO: remove here or not?
    vehicleSet.clear();
}

void VehicleManager::addVehicle(Vehicle* vehicle)
{
    vehicleSet.insert(vehicle);
}

void VehicleManager::removeVehicle(Vehicle* vehicle)
{
    vehicleSet.erase(vehicle);
}
