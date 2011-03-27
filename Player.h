
#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <string>
#include "Player_defs.h"
#include "Vehicle.h"
//#include <assert.h>

//class Vehicle;

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

    Vehicle* getVehicle(); // inline

    const irr::core::matrix4& getViewPos() const; // inline
    const irr::core::matrix4& getViewDest() const; // inline
    const irr::core::matrix4& getVehicleMatrix() const; // inline
    float getVehicleSpeed() const; // inline
    int getVehicleGear() const; // inline

    unsigned int getViewNum() const; // inline
    unsigned int getViewMask() const; // inline
    bool getRecenterView() const; // inline
    void clearRecenterView(); // inline
    void lookLeft(bool set); // inline
    void lookRight(bool set); // inline
    void lookCenter(bool set); // inline
    void switchToNextView(); // inline

private:
    Vehicle*        vehicle;
    std::string     vehicleTypeName;
    unsigned int    viewNum;
    unsigned int    viewMask;
    bool            recenterView;
};

inline Vehicle* Player::getVehicle()
{
    return vehicle;
}

inline const irr::core::matrix4& Player::getViewPos() const
{
    //assert(vehicle);
    return vehicle->getViewPos(CALC_VIEW(viewNum, viewMask));
}

inline const irr::core::matrix4& Player::getViewDest() const
{
    //assert(vehicle);
    return vehicle->getViewDest(CALC_VIEW(viewNum, viewMask));
}

inline const irr::core::matrix4& Player::getVehicleMatrix() const
{
    //assert(vehicle);
    return vehicle->getMatrix();
}

inline float Player::getVehicleSpeed() const
{
    //assert(vehicle);
    return vehicle->getSpeed();
}
inline int Player::getVehicleGear() const
{
    //assert(vehicle);
    return vehicle->getGear();
}

inline unsigned int Player::getViewNum() const
{
    return viewNum;
}

inline unsigned int Player::getViewMask() const
{
    return viewMask;
}

inline bool Player::getRecenterView() const
{
    return recenterView;
}

inline void Player::clearRecenterView()
{
    recenterView = false;
}

inline void Player::lookLeft(bool set)
{
    if (set)
    {
        viewMask |= VIEW_LEFT;
    }
    else
    {
        viewMask &= ~VIEW_LEFT;
    }
    recenterView = true;
}

inline void Player::lookRight(bool set)
{
    if (set)
    {
        viewMask |= VIEW_RIGHT;
    }
    else
    {
        viewMask &= ~VIEW_RIGHT;
    }
    recenterView = true;
}

inline void Player::lookCenter(bool set)
{
    viewMask = VIEW_CENTER;
    recenterView = true;
}

inline void Player::switchToNextView()
{
    SWITCH_TO_NEXT_VIEW(viewNum);
    recenterView = true;
}

#endif // PLAYER_H
