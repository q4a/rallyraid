
#ifndef OBJECTPOOLMANAGER_H
#define OBJECTPOOLMANAGER_H

#include <string>
#include <map>

class ObjectPool;

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
    
private:
    objectPoolMap_t objectPoolMap;
};

#endif // OBJECTPOOLMANAGER_H
