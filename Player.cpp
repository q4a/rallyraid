

#include "Player.h"
#include "Vehicle.h"
#include "Competitor.h"
#include <assert.h>


Player* Player::player = 0;

void Player::initialize()
{
    if (player == 0)
    {
        player = new Player();
    }
}

void Player::finalize()
{
    if (player)
    {
        delete player;
        player = 0;
    }
}

    
Player::Player()
    : vehicle(0),
      competitor(new Competitor(499, "Player", "-", "Player's Team", "vw3", 0, false)),
      viewNum(VIEW_0),
      viewMask(VIEW_CENTER),
      recenterView(true)
{
}

Player::~Player()
{
    finalizeVehicle();
    delete competitor;
}


void Player::initializeVehicle(const std::string& vehicleTypeName, const irr::core::vector3df& apos, const irr::core::vector3df& rotation)
{
    assert(vehicle == 0);
    competitor->setVehicleTypeName(vehicleTypeName);
    vehicle = new Vehicle(vehicleTypeName, apos, rotation);
    recenterView = true;
}

void Player::finalizeVehicle()
{
    if (vehicle)
    {
        delete vehicle;
        vehicle = 0;
    }
}
