
#ifndef OBJECTPOOLMANAGER_H
#define OBJECTPOOLMANAGER_H

#include <string>
#include <map>
#include <irrlicht.h>

class ObjectPool;
class OffsetObject;

class ObjectPoolManager
{
public:
    static void initialize();
    static void finalize();
    
    static ObjectPoolManager* getInstance();

    typedef std::map<std::string, ObjectPool*> objectPoolMap_t;

private:
    static ObjectPoolManager* objectPoolManager;

private:
    ObjectPoolManager();
    ~ObjectPoolManager();

    void read();

public:
    OffsetObject* getObject(const std::string& objectPoolName, const irr::core::vector3df& apos, const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f), bool addToOffsetManager = true);
    void putObject(OffsetObject* offsetObject);

private:
    objectPoolMap_t objectPoolMap;
};

#endif // OBJECTPOOLMANAGER_H
