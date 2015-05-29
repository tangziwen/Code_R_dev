#ifndef ENEMY_TYPE_MGR_H
#define ENEMY_TYPE_MGR_H
#include <vector>
#include "EnemyType.h"
class EnemyTypeMgr{
public:
    static EnemyTypeMgr * get();

    void loadConfig(const char * configFile);
    EnemyType * findItem(unsigned int id);
    EnemyType * findItem(const char* name);
private:
    static EnemyTypeMgr * instance;
    EnemyTypeMgr();
private:
    std::vector<EnemyType *> m_itemList;
};


#endif