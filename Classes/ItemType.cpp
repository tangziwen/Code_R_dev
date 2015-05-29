#include "ItemType.h"

std::string ItemType::getName()
{
    return m_name;
}

void ItemType::setName(std::string newName)
{
    m_name = newName;
}

std::string ItemType::getDescription()
{
    return m_description;
}

void ItemType::setItemDescription(std::string new_description)
{
    m_description = new_description;
}

std::string ItemType::getResPath()
{
    return m_resPath;
}

void ItemType::setResPath(std::string new_path)
{
    m_resPath = new_path;
}

void ItemType::use()
{
    
}

void ItemType::drop()
{

}

ItemType::ItemType(unsigned int ID)
    :m_id(ID)
{

}
