#include "GameClient.h"

#include <network/AssignEvent.h>
#include <network/DestroyEvent.h>
#include <network/DisconnectEvent.h>
#include <network/EmptyEvent.h>
#include <network/EquipEvent.h>
#include <network/ExplosionEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerStatusEvent.h>
#include <network/PositionEvent.h>
#include <network/RotationEvent.h>
#include <network/ScoreEvent.h>
#include <network/SpawnEvent.h>
#include <network/AmmoAmountEvent.h>


#include "NetworkServices.h"
#include "ClientEventHandlerLookup.h"

GameClient::GameClient(const ClientEventHandlerLookup &eventHandlerLookup)
        : eventHandlerLookup(eventHandlerLookup),
          currentPlayerEntityId(0),
          assignedEntity(false) {
}

bool GameClient::connectToServer(const std::string &serverAddress, const int serverPort) {
    network.connectToServer(serverAddress, serverPort);
    return network.isConnected();
}

bool GameClient::disconnectFromServer() {
    assignedEntity = false;//next time we connect, we need to obtain a new entity
    network.disconnectFromServer();
    return !network.isConnected();
}

void GameClient::receive() {
    static char networkData[MAX_PACKET_SIZE];

    int len = network.receivePackets(networkData);

    if (len == 0) {
        printf("<Client> Server is not responding switch back to editor");
        network.disconnectFromServer();
        return;
    }

    if (len < 0) {
        return;
    }

    unsigned int i = 0;
    while (i < (unsigned int)len) {
        EmptyEvent emptyEvent;
        emptyEvent.deserialize(&networkData[i]);

       // printf("eventType is %d\n", emptyEvent.getOpcode());
        //printf("byteSize is %d\n", emptyEvent.getByteSize());

        switch (emptyEvent.getOpcode()) {
            case EVENT_ASSIGN: {
                AssignEvent assignEvent;
                assignEvent.deserialize(&networkData[i]);

                if (!assignedEntity) {
                    printf("<Client> Got Assigned a new EntityId %d\n", assignEvent.getEntityId());
                    currentPlayerEntityId = assignEvent.getEntityId(); //set entityId the client needs to keep track of
                }

                break;
            }
            case EVENT_DISCONNECT: {
                DisconnectEvent disconnectEvent;
                disconnectEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(disconnectEvent);
                break;
            }
            case EVENT_SPAWN: {
                SpawnEvent spawnEvent;
                spawnEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(spawnEvent);
                break;
            }
            case EVENT_DESTROY: {
                DestroyEvent destroyEvent;
                destroyEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(destroyEvent.getOpcode())->handle(destroyEvent);
                break;
            }
            case EVENT_POSITION: {
                PositionEvent positionEvent;
                positionEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(positionEvent);
                break;
            }
            case EVENT_ROTATION: {
                RotationEvent rotationEvent;
                rotationEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(rotationEvent);
                break;
            }
            case EVENT_EXPLOSION: {
                ExplosionEvent explosionEvent;
                explosionEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(explosionEvent);
                break;
            }
            case EVENT_HEALTH: {
                HealthEvent healthEvent;
                healthEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(healthEvent);
                break;
            }
            case EVENT_AMMO_COUNT: {
                AmmoAmountEvent ammoAmountEvent;
                ammoAmountEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(ammoAmountEvent);
                break;
            }
            case EVENT_SCORE: {
                ScoreEvent scoreEvent;
                scoreEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(scoreEvent);
                break;
            }
            case EVENT_PLAYER_STATUS: {
                PlayerStatusEvent playerStatusEvent;
                playerStatusEvent.deserialize(&networkData[i]);
                eventHandlerLookup.find(emptyEvent.getOpcode())->handle(playerStatusEvent);
                break;
            }
            default: {
                printf("error in packet event types: %d\n", emptyEvent.getOpcode());
                return;
            }
        }

        i += emptyEvent.getByteSize();
    }
}

unsigned int GameClient::getCurrentPlayerEntityId() const {
    return currentPlayerEntityId;
}

bool GameClient::getAssignedEntity() const {
    return assignedEntity;
}

void GameClient::sendMessage(const Event &evt) const {
    const unsigned int size = evt.getByteSize();
    char *data = new char[size];

    evt.serialize(data);

    network.sendMessage(data, size);

    delete[] data;
}

void GameClient::sendEquipEvent(EntityType type) const {
    EquipEvent evt(0, type);
    sendMessage(evt);
}


bool GameClient::getIsConnectedToServer() const {
    return network.isConnected();
}
