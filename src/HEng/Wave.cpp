#include "../../includes/HEng/Wave.h"
#include "../../includes/Entities/Robor.h"
#include "../../includes/Entities/Roborto.h"
#include "../../includes/Entities/Robro.h"
#include "../../includes/Entities//Romo.h"
#include "../../includes/Entities/Roo.h"

Wave::Wave(GlobalGameState& ggs, Player& timpy, Level& level, const int waveNumber) : ggs(ggs), player(timpy), level(level), waveNumber(waveNumber) {
    createEnemies();

    allCharacterEntities.push_back(timpy.getEntity());
    for(auto& entity : entities) {
        allCharacterEntities.push_back(entity.get());
    }

}

void Wave::render() {
	level.render();
    for (auto & bullet : bullets) {
        bullet.render();
    }
}

bool Wave::runWave() {
    bool playerDamaged = false;
    enemiesAlive = 0;

    timeSinceLoad += ggs.dt;
    if(timeSinceLoad > 1) {
        waveStarted = true;
    }

    for(auto& bullet : bullets) {
        bullet.move(ggs.dt, level.getPlatforms(), ggs.developerMode);
    }

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
            if( Entity::isColliding(enemy->getEntity()->getRect(),player.getHitRect())) {
                if(player.getEntity()->getRect().y + (player.getEntity()->getRect().h-enemy->getEntity()->getRect().h) < enemy->getEntity()->getRect().y
                    && player.getAbility() == Ability::bounce && player.isCharged()) {
                    player.getEntity()->setYVelocity(-1800);
                    enemy->getEntity()->damage(5);
                    player.setInvincible(true);
                    abilityDamgage = true;
                } else if(!player.isInvincible()) {
                    int randomNumber = rand() % 100;
                    if(player.damage()) {
                        //playerAlive = false;
                        waveNumber = 0;
                    }
                    SDL_GameControllerRumble( ggs.controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                    ggs.updateText = true;
                    playerDamaged = true;
                    enemy->getEntity()->damage(5);
                }
            }
            for(auto bit = bullets.begin(); bit != bullets.end();) {
                if(ggs.developerMode) {
                    // TODO: Add back in developer mode features
                    // TODO: See if I can seperate the rendering of developer mode stuff and the controller
                    SDL_SetRenderDrawColor(ggs.renderer,0,0,255,255);
                    SDL_Rect temp = bit->getTrailingRect();
                    SDL_RenderFillRect(ggs.renderer, &temp);
                }
                if(Entity::isColliding(enemy->getEntity()->getRect(),bit->getTrailingRect())) {
                    //explosion.play();
                    if(bit->decreaseStrength()) {
                        eBullets.erase(bit->getIterator());
                        bit = bullets.erase(bit);
                    } else {
                        ++bit;
                    }
                    enemy->getEntity()->damage(player.getWeapon()->getDamage());
                    break;
                }
                ++bit;
            }
            if(!enemy->getEntity()->isAlive()) {
                explosions.emplace_back(enemy->getEntity()->getRect().x+enemy->getEntity()->getRect().w/2,enemy->getEntity()->getRect().y+enemy->getEntity()->getRect().h/2,ggs.renderer);
                if(!playerDamaged) {
                    player.changeXP(enemy->getDifficulty());
                    player.killEnemy();
                    if(!abilityDamgage) {
                        if(player.getAbilityKills() < ggs.abilityProperties[player.getAbility()][ggs.abilityLevels[player.getAbility()]][1] && player.getAbility() != none && !player.isCharged())
                            player.abilitiesKills++;
                    }
                }
                ggs.updateText = true;
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

    for(auto it = explosions.begin();it != explosions.end();) {
        if(it->finished()) {
            it = explosions.erase(it);
        } else {
            it->render();
            it->advance();
            ++it;
        }
    }

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

