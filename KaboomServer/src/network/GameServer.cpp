#include "GameServer.h"

#include <core/Entity.h>
#include <core/CharacteristicComponent.h>
#include <core/PositionComponent.h>
#include <network/EmptyEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/SpawnEvent.h>
#include <network/AssignEvent.h>

#include "../core/Game.h"

unsigned int GameServer::client_id;

GameServer::GameServer(ConfigSettings * config, const ServerEventHandlerLookup &eventHandlerLookup)
    : eventHandlerLookup(eventHandlerLookup) {
    // id's to assign clients for our table
    client_id = 0;

    printf("<Server> Creating a Network Server\n");
    // set up the server network to listen 
    network = new ServerNetwork(config);

    printf("<Sevrer> Initializing a Game\n");
    int maxPlayers;
    config->getValue(ConfigSettings::str_max_client, maxPlayers);
}

GameServer::~GameServer() {
}

bool GameServer::acceptNewClient(unsigned int entity_id) {
    if (network->acceptNewClient(entity_id)) {
        printf("<Server> Client %d has connected to the server\n", entity_id);

        //client_id++;

        return true;
    }

    return false;
}

void GameServer::receive(Game *game) {
    char network_data[MAX_PACKET_SIZE];

    for (auto it : network->sessions) {
        if (network->sessions.empty()) {
            break;
        }

		int id = it.first;
        int len = network->receiveData(it.first, network_data);

		if (len == 0){
			
			//handle player disconnect by sending delete messages to all clients about the deleted users entities
			Entity *entity = game->getEntityManager().getEntity(id);

            // HACK
            if (entity == nullptr) {
                continue;
            }

			if (entity->getComponent<BombContainerComponent>() == nullptr) {
				printf("\nsomething wrong with look up method\n");
				continue;
			}
			DeleteEvent deleteEvent(id);
			char del[sizeof(DeleteEvent)];
			deleteEvent.serialize(del);
			network->sendToAll(del,sizeof(DeleteEvent));

			
			//TODO: Need to delete our rigid body and it's collision shape
			PhysicsComponent *phys=game->getEntityManager().getEntity(id)->getComponent<PhysicsComponent>();
			game->getBtDiscreteDynamicsWorld()->removeRigidBody(phys->getRigidBody());
			delete phys->getRigidBody()->getMotionState();
			delete phys->getRigidBody();

			game->getEntityManager().destroyEntity(id);
		}

        if (len <= 0) {
            continue;
        }

        EmptyEvent emptyEvent;
		PlayerInputEvent playerInputEvent;
		bool receivedPlayerInputEvent = false;

        unsigned int i = 0;
        while (i < (unsigned int)len) {
            emptyEvent.deserialize(&network_data[i]);

            switch (emptyEvent.getOpcode()) {
            case EventOpcode::PLAYER_INPUT:
                playerInputEvent.deserialize(&network_data[i]);

				playerInputEvent.setPlayerId(it.first); //Important prevent hacking from client impersonating as other clients
				receivedPlayerInputEvent = true;

                break;
            default:
                printf("<Server> error in packet types\n");
                break;
            }

            i += emptyEvent.getByteSize();
        }

		if (receivedPlayerInputEvent) { 
			eventHandlerLookup.find(emptyEvent.getOpcode())->handle(playerInputEvent);
		}
    }
}

void GameServer::sendGameStatePackets(Game *game) const {
    for (Entity *entity : game->getEntityManager().getEntityList()) {
        sendPositionEvent(entity);
    }
}

void GameServer::sendEntitySpawnEvent(Entity* newEntity) const {
	//send the spawn entity event to all the clients
	PositionComponent *positionCom = newEntity->getComponent<PositionComponent>();
    CharacteristicComponent *charCom = newEntity->getComponent<CharacteristicComponent>();

    if (positionCom == nullptr || charCom == nullptr) {
		return;
	}

    SpawnEvent playerSpawnEvent(newEntity->getId(), positionCom->getX(), positionCom->getY(), positionCom->getZ(), charCom->getType(), 0);

    const unsigned int packet_size = sizeof(SpawnEvent);
	char packet_data[packet_size];

	playerSpawnEvent.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);

}

void GameServer::sendAllEntitiesSpawnEvent(Entity* newEntity,std::vector<Entity *> existingEnities) const {
	PositionComponent *positionCom = newEntity->getComponent<PositionComponent>();
	
	const unsigned int packet_size = sizeof(SpawnEvent);
	char packet_data[packet_size];
	for (int i = 0; i < existingEnities.size(); i++){
		if (existingEnities[i]->getId() == newEntity->getId()){
			continue;
		}
		positionCom = existingEnities[i]->getComponent<PositionComponent>();
		EntityType type = EntityType::UNINITIATED;
		if (existingEnities[i]->getComponent<CharacteristicComponent>() != nullptr){
			type = existingEnities[i]->getComponent<CharacteristicComponent>()->getType();
		}
        SpawnEvent extraEntity(existingEnities[i]->getId(), positionCom->getX(), positionCom->getY(), positionCom->getZ(), type, 0);
		extraEntity.serialize(packet_data);
		network->sendToOneClient(packet_data, packet_size, newEntity->getId());
	}
}

void GameServer::sendPositionEvent(Entity* entity) const {
    PositionComponent *positionCom = entity->getComponent<PositionComponent>();

    if (positionCom == nullptr) {
        return;
    }

    PositionEvent positionEvent(entity->getId(), positionCom->getX(), positionCom->getY(), positionCom->getZ());

    const unsigned int packet_size = sizeof(PositionEvent);
    char packet_data[packet_size];

    positionEvent.serialize(packet_data);
    network->sendToAll(packet_data, packet_size);
}


void GameServer::sendAssignPlayerEntity(unsigned int entityId) {

	AssignEvent assignEvent(entityId);
	const unsigned int packet_size = sizeof(AssignEvent);
	char packet_data[packet_size];

	assignEvent.serialize(packet_data);
	network->sendToOneClient(packet_data, packet_size, entityId);

}