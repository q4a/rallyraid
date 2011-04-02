#ifndef COMPETITORS_H
#define COMPETITORS_H

#include <string>


class Competitor
{
public:
    Competitor(unsigned int num,
        const std::string& name,
        const std::string& coName,
        const std::string& teamName,
        const std::string& vehicleTypeName,
        unsigned int strength,
        bool ai = true);
    
    unsigned int getNum(); // inline
    const std::string& getName(); // inline
    const std::string& getCoName(); // inline
    const std::string& getTeamName(); // inline
    const std::string& getVehicleTypeName(); // inline
    unsigned int getStrength(); // inline
    bool getAi(); // inline

private:
    unsigned int num;
    std::string name;
    std::string coName;
    std::string teamName;
    std::string vehicleTypeName;
    unsigned int strength;
    bool ai;
};

inline unsigned int Competitor::getNum()
{
    return num;
}

inline const std::string& Competitor::getName()
{
    return name;
}

inline const std::string& Competitor::getCoName()
{
    return coName;
}

inline const std::string& Competitor::getTeamName()
{
    return teamName;
}

inline const std::string& Competitor::getVehicleTypeName()
{
    return vehicleTypeName;
}

inline unsigned int Competitor::getStrength()
{
    return strength;
}

inline bool Competitor::getAi()
{
    return ai;
}

#endif // COMPETITORS_H
