#ifndef STRINGCONVERTER_H
#define STRINGCONVERTER_H

#include <string>
#include <sstream>

class StringConverter
{
public:
    static float StringConverter::parseFloat(const std::string& val, float defaultValue)
    {
		std::istringstream str(val);
		float ret = defaultValue;
		str >> ret;

        return ret;
    }

    static int parseInt(const std::string& val, int defaultValue)
    {
		std::istringstream str(val);
		int ret = defaultValue;
		str >> ret;

        return ret;
    }

    static unsigned int parseUnsignedInt(const std::string& val, unsigned int defaultValue)
    {
        std::istringstream str(val);
		unsigned int ret = defaultValue;
		str >> ret;

		return ret;
    }
};

#endif // STRINGCONVERTER_H
