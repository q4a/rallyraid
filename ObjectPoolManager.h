
#ifndef OBJECTPOOLMANAGER_H
#define OBJECTPOOLMANAGER_H

class ObjectPoolManager
{
public:
    static void initialize();
    static void finalize();
    
    static ObjectPoolManager* getInstance();

private:
    static ObjectPoolManager* objectPoolManager;

private:
    ObjectPoolManager();
    ObjectPoolManager();
    
private:    
};

#endif // OBJECTPOOLMANAGER_H
