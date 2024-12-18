#include "../includes/Wave.h"
#include "../includes/Robor.h"
#include "../includes/Roborto.h"
#include "../includes/Robro.h"
#include "../includes/Romo.h"
#include "../includes/Roo.h"

Wave::Wave(GlobalGameState& ggs, Player& timpy, Level& level, const int waveNumber) : ggs(ggs), player(timpy), level(level), waveNumber(waveNumber) {
    createEnemies();

    allCharacterEntities.push_back(timpy.getEntity());
    for(auto& entity : entities) {
        allCharacterEntities.push_back(entity.get());
    }

    timeToShootBack.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToShootBack.y = WINDOW_HEIGHT-scalePlayerUI(50);
    timeToShootBack.w = scalePlayerUI(75);
    timeToShootBack.h = scalePlayerUI(15);

    timeToShoot.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToShoot.y = WINDOW_HEIGHT-scalePlayerUI(50);
    timeToShoot.w = scalePlayerUI(75);
    timeToShoot.h = scalePlayerUI(15);

    timeToAbilityBack.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToAbilityBack.y = WINDOW_HEIGHT-scalePlayerUI(75);
    timeToAbilityBack.w = scalePlayerUI(75);
    timeToAbilityBack.h = scalePlayerUI(15);

    timeToAbility.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToAbility.y = WINDOW_HEIGHT-scalePlayerUI(75);
    timeToAbility.w = scalePlayerUI(75);
    timeToAbility.h = scalePlayerUI(15);

    shieldBackRect.x = scalePlayerUI(10);
    shieldBackRect.y = WINDOW_HEIGHT-scalePlayerUI(40);
    shieldBackRect.w = scalePlayerUI(75);
    shieldBackRect.h = scalePlayerUI(15);

    shieldRect.x = scalePlayerUI(10);
    shieldRect.y = WINDOW_HEIGHT-scalePlayerUI(40);
    shieldRect.w = scalePlayerUI(75);
    shieldRect.h = scalePlayerUI(15);

    healthBackRect.x = scalePlayerUI(10);
    healthBackRect.y = WINDOW_HEIGHT-scalePlayerUI(20);
    healthBackRect.w = scalePlayerUI(75);
    healthBackRect.h = scalePlayerUI(15);

    healthRect.x = scalePlayerUI(10);
    healthRect.y = WINDOW_HEIGHT-scalePlayerUI(20);
    healthRect.w = scalePlayerUI(75);
    healthRect.h = scalePlayerUI(15);

    healthText.setup(ggs.renderer);
    healthText.loadFromRenderedText("200", ggs.black,ggs.verySmall);

    shieldText.setup(ggs.renderer);
    shieldText.loadFromRenderedText("0", ggs.black,ggs.verySmall);

}

void Wave::updatePlayerUIText() {
    healthText.loadFromRenderedText(std::to_string(player.getHealth()), ggs.black, ggs.verySmall);
    shieldText.loadFromRenderedText(std::to_string(player.getShield()), ggs.black, ggs.verySmall);
}

void Wave::renderPlayerUI() {
    int bulletsInClip = player.getWeapon()->getBulletsInClip();
    SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(ggs.renderer,&timeToShootBack);

    SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ggs.renderer,&timeToShoot);

    if(player.getAbility() != none) {
        SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
        SDL_RenderFillRect(ggs.renderer,&timeToAbilityBack);
        SDL_SetRenderDrawColor(ggs.renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(ggs.renderer,&timeToAbility);
    }

    healthRect.w = scalePlayerUI(player.getHealthPercentage()*75);

    shieldRect.w = scalePlayerUI(player.getShieldPercentage()*75);

    SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(ggs.renderer,&healthBackRect);
    SDL_RenderFillRect(ggs.renderer,&shieldBackRect);

    SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ggs.renderer,&healthRect);

    SDL_SetRenderDrawColor(ggs.renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(ggs.renderer,&shieldRect);

    healthText.render(scalePlayerUI(12),WINDOW_HEIGHT-scalePlayerUI(19));
    shieldText.render(scalePlayerUI(12),WINDOW_HEIGHT-scalePlayerUI(39));

    for(int i = 0; i < player.getWeapon()->getClipSize(); i++) {
        if(bulletsInClip>i) {
            SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
        }
        SDL_Rect tempRect;
        tempRect.x = WINDOW_WIDTH-scalePlayerUI(30)-scalePlayerUI(20*i);
        tempRect.y = WINDOW_HEIGHT-scalePlayerUI(25);
        tempRect.w = scalePlayerUI(15);
        tempRect.h = scalePlayerUI(15);
        SDL_RenderFillRect(ggs.renderer,&tempRect);
    }

}

void Wave::render() {
	level.render();
    for (auto & bullet : bullets) {
        bullet.render();
    }
    renderPlayerUI();
}

bool Wave::runWave() {
    bool playerDamaged = false;
    enemiesAlive = 0;

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
            if(!enemy->didAlreadyCollide() && player.getWeapon()->getType() == Weapon_Type::knife && Entity::isColliding(enemy->getEntity()->getRect(),player.getWeaponRect())) {
                enemy->getEntity()->damage(player.getWeapon()->getDamage());
                enemy->knifeColliding();
            } else {
                //TODO: Remove knife
                if(player.getWeapon()->getType() == Weapon_Type::knife && !Entity::isColliding(enemy->getEntity()->getRect(),player.getWeaponRect())) {
                    enemy->knifeNotColliding();
                }
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
                            updatePlayerUIText();
                            playerDamaged = true;
                            enemy->getEntity()->damage(5);
                        }

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
                        if(player.getAbilityKills() < player.abilityProperties[player.getAbility()][player.abilityLevels[player.getAbility()]][1] && player.getAbility() != none && !player.isCharged())
                            player.abilitiesKills++;
                    }
                }
                updatePlayerUIText();
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

