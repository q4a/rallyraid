
#include "VehicleType.h"
#include "VehicleTypeManager.h"
#include "ConfigFile.h"
#include "stdafx.h"

VehicleType::VehicleType(const std::string& vehicleTypeName, const std::string& vehicleTypeFilename, bool& ret)
    : vehicleTypeName(vehicleTypeName)
{
    ret = read(vehicleTypeFilename);
}

VehicleType::~VehicleType()
{
}

bool VehicleType::read(const std::string& vehicleTypeFilename)
{

    ConfigFile cf;
    cf.load(vehicleTypeFilename);

    dprintf(MY_DEBUG_NOTE, "Read vehicle type file: %s\n", vehicleTypeName.c_str());
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string sectionName, keyName, valueName;
    while (seci.hasMoreElements())
    {
        sectionName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tVehicle section: %s\n", sectionName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        for (ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valueName = i->second;
            
            dprintf(MY_DEBUG_NOTE, "\t\t%s = %s\n", keyName.c_str(), valueName.c_str());
        }
        
    }
    return true;
}

