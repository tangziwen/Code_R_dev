#include "EnemyTypeMgr.h"
#include "tinyxml2/tinyxml2.h"
EnemyTypeMgr * EnemyTypeMgr::get()
{
    if (!instance)
    {
        instance = new EnemyTypeMgr();
    }
    return instance;
}

void EnemyTypeMgr::loadConfig(const char * configFile)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(configFile);
    auto root = doc.FirstChildElement("Enemies");
    auto itemElement = root->FirstChildElement();
    while (itemElement)
    {
        auto item = new EnemyType(m_itemList.size());
        item->setName(itemElement->FirstChildElement("name")->GetText());
        item->setItemDescription(itemElement->FirstChildElement("description")->GetText());
        auto resPath = std::string("enemies/res/");
        resPath.append(itemElement->FirstChildElement("res_path")->GetText());
        item->setResPath(resPath);

        int damage = atoi(itemElement->FirstChildElement("damage")->GetText());
        item->setDamage(damage);

        int hp = atoi(itemElement->FirstChildElement("health")->GetText());
        item->setHp(hp);

        m_itemList.push_back(item);
        itemElement = itemElement->NextSiblingElement();
    }
}

EnemyType * EnemyTypeMgr::findItem(unsigned int id)
{
    return m_itemList[id];
}

EnemyType * EnemyTypeMgr::findItem(const char* name)
{
    for (int i = 0; i < m_itemList.size(); i++)
    {
        if (m_itemList[i]->getName().compare(name) == 0)
        {
            return m_itemList[i];
        }
    }
}

EnemyTypeMgr::EnemyTypeMgr()
{

}

EnemyTypeMgr * EnemyTypeMgr::instance = NULL;
