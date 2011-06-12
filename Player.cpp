

#include "Player.h"
#include "Vehicle.h"
#include "Competitor.h"
#include "RaceEngine.h"
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
      starter(0),
      viewNum(VIEW_0),
      viewMask(VIEW_CENTER),
      recenterView(true),
      firstPressed(false)
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
    firstPressed = false;
}

void Player::finalizeVehicle()
{
    if (vehicle)
    {
        delete vehicle;
        vehicle = 0;
    }
}

bool Player::save(const std::string& filename)
{
    FILE* f;
    int ret = 0;

    errno_t error = fopen_s(&f, filename.c_str(), "w");
    if (error)
    {
        printf("unable to open player file for write %s\n", filename.c_str());
        return false;
    }

    ret = fprintf(f, "%s\n", competitor->getName().c_str());
    ret = fprintf(f, "%s\n", competitor->getTeamName().c_str());
    ret = fprintf(f, "%s\n", competitor->getVehicleTypeName().c_str());

    fclose(f);

    return (ret > 0);
}

bool Player::load(const std::string& filename)
{
    FILE* f;
    int ret = 0;
    char name[256];
    char teamName[256];
    char vehicleTypeName[256];

    errno_t error = fopen_s(&f, filename.c_str(), "r");
    if (error)
    {
        printf("player file unable to open: %s\n", filename.c_str());
        return false;
    }
    
    ret = fscanf_s(f, "%s\n", name);
    if (ret < 1)
    {
        printf("player file unable to read player name: %s\n", filename.c_str());
        fclose(f);
        return false;
    }

    ret = fscanf_s(f, "%s\n", teamName);
    if (ret < 1)
    {
        printf("player file unable to read team name: %s\n", filename.c_str());
        fclose(f);
        return false;
    }

    ret = fscanf_s(f, "%s\n", vehicleTypeName);
    if (ret < 1)
    {
        printf("player file unable to read vehicle type name: %s\n", filename.c_str());
        fclose(f);
        return false;
    }

    fclose(f);
    competitor->setName(name);
    competitor->setTeamName(teamName);
    competitor->setVehicleTypeName(vehicleTypeName);
    return true;
}

