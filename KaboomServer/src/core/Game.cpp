#include "Game.h"

#include <Windows.h>

#include <components/PlayerComponent.h>
#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <core/Player.h>
#include <util/ConfigSettings.h>

#include "../components/PhysicsComponent.h"
#include "../components/RespawnComponent.h"
#include "../components/TriggerComponent.h"
#include "../core/GameModeConfigLoader.h"
#include "../network/GameServer.h"
#include "../network/ServerEventHandlerLookup.h"
#include "../systems/CharacterSpawnSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/DeathSystem.h"
#include "../systems/DestroySystem.h"
#include "../systems/ExplosionSystem.h"
#include "../systems/FiringSystem.h"
#include "../systems/InitializationSystem.h"
#include "../systems/InputSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/PickupSpawnSystem.h"
#include "../systems/PickupSystem.h"
#include "../systems/TimerSystem.h"
#include "../systems/VoidSystem.h"
#include "../systems/JumpPadSystem.h"
#include "../systems/JumpPadSpawnSystem.h"

Game::Game(ConfigSettings *configSettings)
        : characterFactory(entityManager),
          bombFactory(entityManager),
          pickupFactory(entityManager),
          jumpPadFactory(entityManager),
          eventHandlerLookup(this),
          server(configSettings, eventHandlerLookup),
          world(configSettings) {
    std::string mediaPath;
    std::string worldXml;

    configSettings->getValue(ConfigSettings::str_mediaFilePath, mediaPath);
    configSettings->getValue(ConfigSettings::str_world_xml, worldXml);

    worldXml = mediaPath + worldXml;

    std::cout << worldXml << std::endl;

    world.load(worldXml, "data-server/map.xml");

    GameModeConfigLoader gameModeConfigLoader(gameModeConfigs);
    gameModeConfigLoader.load("data-server/game-modes.xml");

    auto deathmatchConfig = gameModeConfigs["Deathmatch"];

    gameMode.setPreMatchDuration(deathmatchConfig.getInt("pre-match-duration") * 1000);
    gameMode.setMatchDuration(deathmatchConfig.getInt("match-duration") * 1000);
    gameMode.setPostMatchDuration(deathmatchConfig.getInt("post-match-duration") * 1000);

    systemManager.addSystem(new InitializationSystem(this));
    systemManager.addSystem(new CharacterSpawnSystem(this));
    systemManager.addSystem(new JumpPadSpawnSystem(this));
    systemManager.addSystem(new PickupSpawnSystem(this));
    systemManager.addSystem(new InputSystem(this));
    systemManager.addSystem(new FiringSystem(this));
    systemManager.addSystem(new PhysicsSystem(this, world));
    systemManager.addSystem(new VoidSystem(this));
    systemManager.addSystem(new CollisionSystem(this));
    systemManager.addSystem(new TimerSystem(this));
    systemManager.addSystem(new PickupSystem(this));
    systemManager.addSystem(new ExplosionSystem(this));
    systemManager.addSystem(new JumpPadSystem(this));
    systemManager.addSystem(new DeathSystem(this));
    systemManager.addSystem(new DestroySystem(this));
}

Game::~Game() {
}

void Game::run() {
    const clock_t TICK = 1000 / FPS;
    const float TIME_STEP = 1.0f / FPS;

    while (true) {
        clock_t beginTime = clock();

        update();

        clock_t endTime = clock();
        clock_t elapsedTime = endTime - beginTime;
        clock_t sleepTime = TICK - elapsedTime;

        if (sleepTime > 0) {
            Sleep(sleepTime);
        } else {
            printf("Warning we need to slow down our server ticks!\n");
        }
    }
}

void Game::update() {
    unsigned int newPlayerId;
    bool hasNewPlayer = server.acceptClient(newPlayerId);

    if (hasNewPlayer) {
        Player *player = new Player(newPlayerId);
        addPlayer(player);
        server.sendConnectEvent(player);
        server.sendAssignEvent(player);
    }

    switch (gameMode.getMatchState()) {
        case GameMode::MatchState::ENTER_MAP: {
            for (auto kv : world.getSpawnPointConfigs()) {
                auto name = kv.first;
                auto config = kv.second;

                if (config.getString("object-type") == "Pickup") {
                    //duration is Zero at first, so the request is immediate and it will spawn right away in Spawn System
                    pickupSpawnRequest[name] = Timer(0);
                } else if (config.getString("object-type") == "Player") {
                    playerSpawnPointList.push_back(name);
                } else if (config.getString("object-type") == "JumpPad") {
                    jumpPadSpawnPointList.push_back(name);
                }
            }
            break;
        }
        case GameMode::MatchState::PRE_MATCH: {
            if (players.empty()) {
                gameMode.setTimer(Timer(gameMode.getPreMatchDuration()));
            }

            for (auto kv : players) {
                const auto player = kv.second;

                if (player->getEntity() == nullptr) {
                    addPlayerToWorld(player);
                }
            }

            server.receive(players);
            systemManager.processSystems(this);
            server.sendGameStatePackets(players, entityManager.getEntityList());
            break;
        }
        case GameMode::MatchState::IN_PROGRESS: {
            if (hasNewPlayer) {
                addPlayerToWorld(players.at(newPlayerId));
            }

            server.receive(players);
            systemManager.processSystems(this);
            server.sendGameStatePackets(players, entityManager.getEntityList());
            break;
        }
        case GameMode::MatchState::POST_MATCH: {
            server.receive(players);
            systemManager.processSystems(this);
            server.sendGameStatePackets(players, entityManager.getEntityList());
            break;
        }
        case GameMode::MatchState::LEAVE_MAP: {
            for (auto entity : entityManager.getEntityList()) {
                removeEntity(entity);
            }

            for (auto kv : players) {
                const auto player = kv.second;
                player->setEntity(nullptr);
            }
            break;
        }
    }

    if (gameMode.updateMatchState()) {
        switch (gameMode.getMatchState()) {
            case GameMode::MatchState::PRE_MATCH:
            case GameMode::MatchState::IN_PROGRESS:
            case GameMode::MatchState::POST_MATCH: {
                server.sendMatchStateEvent(gameMode);
                break;
            }
        }
    }

    world.renderDebugFrame();
}

void Game::addPlayer(Player *player) {
    players[player->getId()] = player;
}

void Game::removePlayer(Player *player) {
    players.erase(players.find(player->getId()));

    delete player;
}

void Game::addEntity(Entity *entity) {
    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.addRigidBody(physicsComp->getRigidBody());
    }

    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    if (triggerComp != nullptr) {
        world.addTrigger(triggerComp->getGhostObject());
    }

    server.sendSpawnEvent(entity);
}

void Game::removeEntity(Entity *entity) {
    server.sendDestroyEvent(entity);

    PhysicsComponent *physicsComp = entity->getComponent<PhysicsComponent>();

    if (physicsComp != nullptr) {
        world.removeRigidBody(physicsComp->getRigidBody());
    }

    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    if (triggerComp != nullptr) {
        world.removeTrigger(triggerComp->getGhostObject());
    }

    entityManager.destroyEntity(entity->getId());
}

Vec3 Game::getPlayerSpawnPoint() {
    auto spawnPoint = playerSpawnPointList.at(rand() % playerSpawnPointList.size());

    Configuration spawnConfig = getSpawnPointConfigs().at(spawnPoint);

    Vec3 pos;
    pos.setOsgVec3(spawnConfig.getVec3("position"));

    return pos;
}

void Game::addPlayerToWorld(Player *player) {
    Entity *entity = characterFactory.createCharacter(DEFAULT_CHARACTER, getPlayerSpawnPoint());
    entity->attachComponent(new PlayerComponent(player));
    player->setEntity(entity);
    player->setKills(0);
    player->setDeaths(0);

    addEntity(entity);
    server.sendBindEvent(player);
    server.sendInitializeEvent(player, players, entityManager.getEntityList());
    server.sendGameStatePackets(player, entityManager.getEntityList());
}
