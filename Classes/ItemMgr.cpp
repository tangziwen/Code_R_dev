#include "ItemMgr.h"
#include "tinyxml2/tinyxml2.h"
ItemMgr * ItemMgr::get()
{
    if (!instance)
    {
        instance = new ItemMgr();
    }
    return instance;
}

void ItemMgr::loadConfig(const char * configFile)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(configFile);
    auto root = doc.FirstChildElement("Items");
    auto itemElement = root->FirstChildElement();
    while (itemElement)
    {
        auto item = new ItemType(m_itemList.size());
        item->setName(itemElement->FirstChildElement("name")->GetText());
        item->setItemDescription(itemElement->FirstChildElement("description")->GetText());
        auto resPath = std::string("items/res/");
        resPath.append(itemElement->FirstChildElement("res_path")->GetText());
        item->setResPath(resPath);
        m_itemList.push_back(item);
        itemElement = itemElement->NextSiblingElement();
    }
}

ItemType * ItemMgr::findItem(unsigned int id)
{
    return m_itemList[id];
}

ItemType * ItemMgr::findItem(const char* name)
{
    for (int i = 0; i < m_itemList.size(); i++)
    {
        if (m_itemList[i]->getName().compare(name) == 0)
        {
            return m_itemList[i];
        }
    }
}

ItemMgr::ItemMgr()
{

}

ItemMgr * ItemMgr::instance = NULL;
