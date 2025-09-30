#ifndef VersionUtils_HPP
#define VersionUtils_HPP

#include <Arduino.h>
#include <vector>

class VersionUtils 
{
    private:
    public:
        static bool isVersionGreaterOrEqual(const std::string & v1, const std::string& v2);
};

#endif // VersionUtils_HPP