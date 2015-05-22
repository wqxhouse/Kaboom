#include "Game.h"

#include <Windows.h>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <core/Player.h>
#include <util/ConfigSettings.h>

#include "../components/PhysicsComponent.h"
#include "../components/RespawnComponent.h"
#include "../components/TriggerComponent.h"
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

Game::Game(ConfigSettings *configSettings)
        : characterFactory(entityManager),
          bombFactory(entityManager),
          pickupFactory(entityManager),
          eventHandlerLookup(this),
          server(configSettings, eventHandlerLookup),
          world(configSettings) {
    std::string str_mediaPath = "";
    std::string str_world_xml = "";

    configSettings->getValue(ConfigSettings::str_mediaFilePath, str_mediaPath);
    configSettings->getValue(ConfigSettings::str_world_xml, str_world_xml);

    str_world_xml = str_mediaPath + str_world_xml;

    std::cout << str_world_xml << std::endl;
    loadWorld(str_world_xml, "data-server/map.xml");

    systemManager.addSystem(new InitializationSystem(this));
    systemManager.addSystem(new CharacterSpawnSystem(this));
    systemManager.addSystem(new PickupSpawnSystem(this));
    systemManager.addSystem(new InputSystem(this));
    systemManager.addSystem(new FiringSystem(this));
    systemManager.addSystem(new PhysicsSystem(this, world));
    systemManager.addSystem(new VoidSystem(this));
    systemManager.addSystem(new CollisionSystem(this));
    systemManager.addSystem(new TimerSystem(this));
    systemManager.addSystem(new PickupSystem(this));
    systemManager.addSystem(new ExplosionSystem(this));
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
    unsigned int playerId;

    if (server.acceptClient(playerId)) {
        Entity *entity = characterFactory.createCharacter(DEFAULT_CHARACTER, getPlayerSpawnPoint());
        Player *player = new Player(playerId, entity);
        addPlayer(player);
        addEntity(entity);

        server.sendConnectEvent(player);
        server.sendBindEvent(player);
        server.sendAssignEvent(player);
        server.sendInitializeEvent(player, entityManager.getEntityList());
        server.sendGameStatePackets(player, entityManager.getEntityList());
    }

    auto &players = getPlayers();

    server.receive(players);
    systemManager.processSystems(this);
    server.sendGameStatePackets(players, entityManager.getEntityList());

    world.renderDebugFrame();
}

void Game::addPlayer(Player *player) {
    players[player->getId()] = player;
    entityIdToPlayerMap[player->getEntity()->getId()] = player;
}

void Game::removePlayer(Player *player) {
    players.erase(players.find(player->getId()));

    for (auto it = entityIdToPlayerMap.begin(); it != entityIdToPlayerMap.end();) {
        if (player == it->second) {
            it = entityIdToPlayerMap.erase(it);
        } else {
            ++it;
        }
    }

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

void Game::loadWorld(const std::string &mapFilename, const std::string &entitiesFilename) {
    world.load(mapFilename, entitiesFilename);

    for (auto kv : world.getSpawnPointConfigs()) {
        auto name = kv.first;
        auto config = kv.second;

        if (config.getString("object-type") == "Pickup") {
            //duration is Zero at first, so the request is immediate and it will spawn right away in Spawn System
            pickupSpawnRequest[name] = Timer(0);
        } else if (config.getString("object-type") == "Player") {
            playerSpawnPointList.push_back(name);
        }
    }
}
