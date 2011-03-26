
#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <string>


class Vehicle;

class Player
{
public:
    static void initialize();
    static void finalize();
    
    static Player* getInstance() {return player;}

private:
    static Player* player;

private:
    Player();
    ~Player();

public:
    void initializeVehicle(const irr::core::vector3df& apos, const irr::core::vector3df& rotation);
    void finalizeVehicle();
    Vehicle* getVehicle();
    

private:
    Vehicle*        vehicle;
    std::string     vehicleTypeName;
};

inline Vehicle* Player::getVehicle()
{
    return vehicle;
}

#endif // PLAYER_H
