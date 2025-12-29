#include "Equipment.h"

Equipment::Equipment()
{
    // Initialize with empty/default items
    helmet = "None";
    armor = "None";
    weapon = "None";
    shield = "None";
    boots = "None";
    ring = "None";
    amulet = "None";
}

Equipment::~Equipment()
{
}

// Getters
std::string Equipment::getHelmet() const { return helmet; }
std::string Equipment::getArmor() const { return armor; }
std::string Equipment::getWeapon() const { return weapon; }
std::string Equipment::getShield() const { return shield; }
std::string Equipment::getBoots() const { return boots; }
std::string Equipment::getRing() const { return ring; }
std::string Equipment::getAmulet() const { return amulet; }

// Setters
void Equipment::setHelmet(const std::string& item) { helmet = item; }
void Equipment::setArmor(const std::string& item) { armor = item; }
void Equipment::setWeapon(const std::string& item) { weapon = item; }
void Equipment::setShield(const std::string& item) { shield = item; }
void Equipment::setBoots(const std::string& item) { boots = item; }
void Equipment::setRing(const std::string& item) { ring = item; }
void Equipment::setAmulet(const std::string& item) { amulet = item; }
