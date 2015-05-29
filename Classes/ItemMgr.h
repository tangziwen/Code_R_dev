#ifndef ITEM_MGR_H
#define ITEM_MGR_H
#include <vector>
#include "ItemType.h"
class ItemMgr{
public:
    static ItemMgr * get();

    void loadConfig(const char * configFile);
    ItemType * findItem(unsigned int id);
    ItemType * findItem(const char* name);
private:
    static ItemMgr * instance;
    ItemMgr();
private:
    std::vector<ItemType *> m_itemList;
};


#endif