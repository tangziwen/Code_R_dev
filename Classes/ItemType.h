#ifndef  ITEM_TYPE_H
#define ITEM_TYPE_H
#include <string>
class ItemType
{
public:
    ItemType(unsigned int ID);
    std::string getName();
    void setName(std::string newName);

    std::string getDescription();
    void setItemDescription(std::string new_description);

    std::string getResPath();
    void setResPath(std::string new_path);

    virtual void use();
    virtual void drop();
private:
    std::string m_name;
    std::string m_description;
    std::string m_resPath;
    unsigned int m_id;
};

#endif