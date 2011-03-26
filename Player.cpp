

#include "Player.h"
#include "Vehicle.h"
#include "assert.h"

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
      vehicleTypeName("vw3")
{
}

Player::~Player()
{
    finalizeVehicle();
}


void Player::initializeVehicle(const irr::core::vector3df& apos, const irr::core::vector3df& rotation)
{
    assert(vehicle == 0);
    vehicle = new Vehicle(vehicleTypeName, apos, rotation);
}

void Player::finalizeVehicle()
{
    if (vehicle)
    {
        delete vehicle;
        vehicle = 0;
    }
}
