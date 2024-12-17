#ifndef PLAYER_H
#define PLAYER_H
#include <list>

#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Sound.h"
#include "Spawn.h"
#include "Weapon.h"

#include <exception>

enum Ability {
    bounce,
    teleport,
    c4,
    grenade,
    none,
};

enum PlayerUpgrades {
    armor,
    shield,
    speed,
    dodge
};

class Player {
public:
    Player(Entity* entity, GlobalGameState& ggs);

    int move(GlobalGameState& ggs ,const std::list<Platform> &platforms);
    void render() const;

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    [[nodiscard]] bool getDirection() const { return playerDirection; }

    [[nodiscard]] Entity* getEntity() const { return playerEntity; }

    Weapon* getWeapon() { return currentWeapon; }
    void setPrimaryWeapon(Weapon* priWeapon) { primaryWeapon = priWeapon; }
    void setSecondaryWeapon(Weapon* secWeapon) { secondaryWeapon = secWeapon; currentWeapon = primaryWeapon; }
    Weapon* getSecondaryWeapon() { return secondaryWeapon; }
    Weapon* getPrimaryWeapon() { return primaryWeapon; }
    void changeWeapon();

    bool damage();
    [[nodiscard]] int getHealth() const { return health; }
    [[nodiscard]] double getHealthPercentage() const { return health/static_cast<double>(maxHealth); }
    [[nodiscard]] int getShield() const { return shield; }
    [[nodiscard]] double getShieldPercentage() const { return shield/static_cast<double>(maxShield); }

    void fullHealth() { health = maxHealth; }
    void fillShield() { shield = maxShield; }
    void reset(State& state);

    void killEnemy();
    void zeroCombo() { combo = 0;}
    int getCombo() const { return combo; }

    void changeXP(int _xp) { xp += _xp; }
    void setXP(int _xp ) { xp = _xp; }
    int getXP() const { return xp; }

    int charge();
    void useAbility(State& state);
    [[nodiscard]] bool isCharged() const { return charged;}
    Ability getAbility() { return currentAbility; }
    void setAbility(Ability ability) { currentAbility = ability; }

    [[nodiscard]] bool isInvincible() const { return invincible; }
    void setInvincible(bool _invincible) { invincible = _invincible; }
    void tickInvicibilty(float dt);

    Entity* getC4Entity() { return &c4Entity; }

    void setXNormalV(int vx) { xNormalVelocity = vx; }

    SDL_Rect getHitRect() { return playerHitRect; }
    SDL_Rect* getWheelRect() { return &wheelRect; }
    void updateWheelRect() {
        wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
        wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;
    }

    [[nodiscard]] int getAbilityKills() const { return abilitiesKills; }

    //TODO: Move out of public space
    const double weaponProperties[5][5][6] = {
        {{10,4,3,1,1,1},{20,5,3,1,1,1},{30,6,2,2,1,1},{40,7,2,2,2,1},{50,8,1,2,2,1}}, //Revolver
        {{10,2,3,1,2,1},{20,2,3,2,2,2},{30,3,2,2,3,2},{40,3,2,2,4,3},{50,4,2,2,5,3}}, //Rifle
        {{10,1,3,1,1,3},{20,1,3,1,1,4},{30,2,2,1,1,5},{40,2,2,1,1,6},{50,3,1,1,1,7}}, //Shotgun
        {{10,1,0,0,0,0},{20,1,0,0,0,0},{30,2,0,0,0,0},{40,2,0,0,0,0},{50,3,0,0,0,0}}, //Knife
        {{10,0.5,2,3,0,0},{20,0.5,3,3,0,0},{30,0.4,4,2,0,0},{40,0.4,5,2,0,0},{50,0.3,6,2,0,0}}}; //Laser Pistol

    const int abilityProperties[4][5][3] = {
        {{10,10,0},{20,8,0},{30,6,0},{40,4,0},{50,2,0}},
        {{10,15,2},{20,12,2},{30,10,3},{40,8,3},{50,5,3}},
        {{10,15,1},{20,15,2},{30,12,3},{40,12,4},{50,10,5}},
        {{10,5,1},{20,4,1},{20,4,2},{40,3,2},{50,3,3}}
    };

    const double playerProperties[4][5][2] = {
        {{10,10},{20,20},{30,30},{40,40},{50,50}}, //Armor
        {{10,1.25},{20,1.5},{30,1.75},{40,2},{50,2.5}}, //Shield Charge
        {{10,10},{20,20},{30,30},{40,40},{50,50}}, //Speed
        {{10,1},{20,2},{30,3},{40,4},{50,5}} // Dodge
    };

    int weaponLevels[5] = {1,0,0,0,0};
    int abilityLevels[4] = {0,0,0,0};
    int playerLevels[4] = {0,0,0,0};

    int abilitiesKills = 0;

private:
    const int playerWidth = scale(50);
    const int playerHeight = scale(60);

    Weapon revolver;
    Weapon rifle;
    Weapon shotgun;
    Weapon knife;
    Weapon laserPistol;

    bool playerDirection = false;

    Entity* playerEntity;

    SDL_Rect playerHitRect;
    SDL_Rect wheelRect;

    Texture playerTexture;

    Weapon* currentWeapon = nullptr;
    Weapon* primaryWeapon = nullptr;
    Weapon* secondaryWeapon = nullptr;

    Ability currentAbility = Ability::none;

    float postDamageInvincibleTime = 0;
    bool invicibleFromDeath = false;

    int health = 200;
    const int maxHealth = 200;
    double shield = 0;
    const int maxShield = 200;

    int xp = 0;

    int combo = 0;

    const int defaultXSpeed = scale(350);
    int xNormalVelocity = 0;

    bool charged = true;
    bool justCharged = false;

    bool invincible = false;

    float timeSinceShot;

    SDL_Rect weaponRect;

    //C4 Ability Variables
    Entity c4Entity;
    Texture c4Texture;
    Texture detinatorTexture;
    bool c4Placed = false;

    Sound damageSound;

    int numberOfPlayerUpgrades = 4;

};

#endif //PLAYER_H
