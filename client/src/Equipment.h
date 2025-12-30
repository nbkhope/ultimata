#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include <string>

/**
 * Equipment class to store equipped items for a player
 */
class Equipment
{
public:
    Equipment();
    ~Equipment();

    // Equipment slot getters
    std::string getHelmet() const;
    std::string getArmor() const;
    std::string getWeapon() const;
    std::string getShield() const;
    std::string getBoots() const;
    std::string getRing() const;
    std::string getAmulet() const;

    // Equipment slot setters
    void setHelmet(const std::string& item);
    void setArmor(const std::string& item);
    void setWeapon(const std::string& item);
    void setShield(const std::string& item);
    void setBoots(const std::string& item);
    void setRing(const std::string& item);
    void setAmulet(const std::string& item);

private:
    std::string helmet;
    std::string armor;
    std::string weapon;
    std::string shield;
    std::string boots;
    std::string ring;
    std::string amulet;
};

#endif
