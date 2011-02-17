#ifndef VEHICLETYPE_H
#define VEHICLETYPE_H

#include <string>
#include <irrlicht.h>

class VehicleType
{
public:
    VehicleType(const std::string& vehicleTypeName, const std::string& vehicleTypeFilename, bool& ret);
    ~VehicleType();

private:
    bool read(const std::string& vehicleTypeFilename);

private:
    std::string         vehicleTypeName;
};

#endif // VEHICLETYPE_H
