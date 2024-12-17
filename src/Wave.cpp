#include "../includes/Wave.h"
#include "../includes/Robor.h"
#include "../includes/Roborto.h"
#include "../includes/Robro.h"
#include "../includes/Romo.h"
#include "../includes/Roo.h"

Wave::Wave(GlobalGameState& ggs, Player& timpy, Level& level, const int waveNumber) : ggs(ggs), timpy(timpy), level(level), waveNumber(waveNumber) {
    createEnemies();

    allCharacterEntities.push_back(timpy.getEntity());
    for(auto& entity : entities) {
        allCharacterEntities.push_back(entity.get());
    }

}

void Wave::render() const {
	level.render();
}

bool Wave::runWave() {
    bool playerDamaged = false;
    enemiesAlive = 0;

    level.updateSpawns(allCharacterEntities);

    for (const auto & enemy : enemies) {
        bool firstLoop = false;
        if(!enemy->getEntity()->isSpawned()) {
            enemy->getEntity()->spawn(enemiesAlive <= 5);
            firstLoop = true;
        }
        if(enemy->getEntity()->isAlive() && enemy->getEntity()->isSpawned()) {
            bool abilityDamgage = false;

            if(!firstLoop && !ggs.pauseEnemies) {
                enemy->move(ggs, level.getPlatforms(),level);
            }
            enemy->render();
            if(!enemy->didAlreadyCollide() && timpy.getWeapon()->getType() == Weapon_Type::knife && Entity::isColliding(enemy->getEntity()->getRect(),timpy.getWeaponRect())) {
                enemy->getEntity()->damage(timpy.getWeapon()->getDamage());
                enemy->knifeColliding();
            } else {
                //TODO: Remove knife
                if(timpy.getWeapon()->getType() == Weapon_Type::knife && !Entity::isColliding(enemy->getEntity()->getRect(),timpy.getWeaponRect())) {
                    enemy->knifeNotColliding();
                }
                if( Entity::isColliding(enemy->getEntity()->getRect(),timpy.getHitRect())) {
                    if(timpy.getEntity()->getRect().y + (timpy.getEntity()->getRect().h-enemy->getEntity()->getRect().h) < enemy->getEntity()->getRect().y
                        && timpy.getAbility() == Ability::bounce && timpy.isCharged()) {
                            timpy.getEntity()->setYVelocity(-1800);
                            enemy->getEntity()->damage(5);
                            timpy.setInvincible(true);
                            abilityDamgage = true;
                        } else if(!timpy.isInvincible()) {
                            int randomNumber = rand() % 100;
                            if(timpy.damage()) {
                                //playerAlive = false;
                                waveNumber = 0;
                            }
                            SDL_GameControllerRumble( ggs.controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                            //updateInGameText(timpy.getCombo(),waveNumber,timpy.getXP(),timpy.getHealth(), timpy.getShield());
                            playerDamaged = true;
                            enemy->getEntity()->damage(5);
                        }

                }
            }
            // TODO: Add bullets in
            /*for(auto bit = bullets.begin(); bit != bullets.end();) {
                if(state.developerMode) {
                    SDL_SetRenderDrawColor(gameRenderer,0,0,255,255);
                    SDL_Rect temp = bit->getTrailingRect();
                    SDL_RenderFillRect(gameRenderer, &temp);
                }
                if(Entity::isColliding(enemy->getEntity()->getRect(),bit->getTrailingRect())) {
                    explosion.play();
                    if(bit->decreaseStrength()) {
                        eBullets.erase(bit->getIterator());
                        bit = bullets.erase(bit);
                    } else {
                        ++bit;
                    }
                    enemy->getEntity()->damage(timpy.getWeapon()->getDamage());
                    break;
                }
                ++bit;
            }*/
            // TODO: Add back in C4
            /*if(state.c4Exploded) {
                int c4x = timpy.getC4Entity()->getRect().x;
                int c4y = timpy.getC4Entity()->getRect().y;
                if(pow(pow(c4x - enemy->getEntity()->getRect().x,2)+pow(c4y - enemy->getEntity()->getRect().y,2),0.5) < scale(200)) {
                    (*it)->getEntity()->damage(state.abilityProperties[c4][state.abilityLevels[c4]-1][2]);
                    abilityDamgage = true;
                }
            }*/
            if(!enemy->getEntity()->isAlive()) {
                //explosions.emplace_back((*it)->getEntity()->getRect().x+(*it)->getEntity()->getRect().w/2,(*it)->getEntity()->getRect().y+(*it)->getEntity()->getRect().h/2,gameRenderer);
                if(!playerDamaged) {
                    timpy.changeXP(enemy->getDifficulty());
                    timpy.killEnemy();
                    if(!abilityDamgage) {
                        if(timpy.getAbilityKills() < timpy.abilityProperties[timpy.getAbility()][timpy.abilityLevels[timpy.getAbility()]][1] && timpy.getAbility() != none && !timpy.isCharged())
                            timpy.abilitiesKills++;
                    }
                }
                //updateInGameText(timpy.getCombo(),waveNumber, timpy.getXP(),timpy.getHealth(), timpy.getShield());
            } else {
                for(auto& teleport : level.getTeleports()) {
                    if(Entity::isColliding(enemy->getEntity()->getRect(),teleport)) {
                        enemy->getEntity()->despawn();
                    }
                }

            }
        }
        if(enemy->getEntity()->isAlive()) {
            enemiesAlive++;
        }
    }

    // TODO: Add in explosions
    /*
    for(auto it = explosions.begin();it != explosions.end();) {
        if(it->finished()) {
            it = explosions.erase(it);
        } else {
            it->render();
            it->advance();
            ++it;
        }
    }
    */

    if(enemiesAlive == 0) {
        return false;
    }

    return true;
}

void Wave::createEnemies() {
	int totalDifficulty = 0;
    int (*weights)[5] = level.getSpawnWeights();
    std::vector<Spawn>* spawns = level.getEnemySpawns();
    while(totalDifficulty < waveNumber) {
        int enemyType = rand() % 100 + 1;
        if(enemyType <= (*weights)[0] && totalDifficulty+Robor::difficulty <= waveNumber) {
            totalDifficulty += Robor::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Robor::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Robor(entities.back().get())));
        } else if(enemyType > (*weights)[0] && enemyType <= (*weights)[1] && totalDifficulty+Roborto::difficulty <= waveNumber) {
            totalDifficulty += Roborto::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Roborto::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Roborto(entities.back().get())));
        } else if(enemyType > (*weights)[1] && enemyType <= (*weights)[2] && totalDifficulty+Robro::difficulty <= waveNumber) {
            totalDifficulty += Robro::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Robro::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Robro(entities.back().get())));
        } else if(enemyType > (*weights)[2] && enemyType <= (*weights)[3] && totalDifficulty+Romo::difficulty <= waveNumber) {
            totalDifficulty += Romo::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Romo::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Romo(entities.back().get())));
        } else if(enemyType > (*weights)[3] && enemyType <= (*weights)[4] && totalDifficulty+Roo::difficulty <= waveNumber) {
            totalDifficulty += Roo::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Roo::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Roo(entities.back().get())));
        }
    }
}

