#include "GameClient.h"

#include <network/AmmoAmountEvent.h>
#include <network/AssignEvent.h>
#include <network/BindEvent.h>
#include <network/ConnectEvent.h>
#include <network/DestroyEvent.h>
#include <network/DisconnectEvent.h>
#include <network/EmptyEvent.h>
#include <network/EquipEvent.h>
#include <network/ExplosionEvent.h>
#include <network/PlayerInputEvent.h>
#include <network/PlayerStatusEvent.h>
#include <network/PositionEvent.h>
#include <network/ReloadRequestEvent.h>
#include <network/RotationEvent.h>
#include <network/ScoreEvent.h>
#include <network/SpawnEvent.h>

#include "NetworkServices.h"
#include "ClientEventHandlerLookup.h"

GameClient::GameClient(const ClientEventHandlerLookup &eventHandlerLookup)
        : eventHandlerLookup(eventHandlerLookup) {
}

bool GameClient::connectToServer(const std::string &serverAddress, const int serverPort) {
    network.connectToServer(serverAddress, serverPort);
    return network.isConnected();
}

bool GameClient::disconnectFromServer() {
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
            case EVENT_CONNECT: {
                ConnectEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_DISCONNECT: {
                DisconnectEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_ASSIGN: {
                AssignEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_BIND: {
                BindEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_SPAWN: {
                SpawnEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_DESTROY: {
                DestroyEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_POSITION: {
                PositionEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_ROTATION: {
                RotationEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_EXPLOSION: {
                ExplosionEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_HEALTH: {
                HealthEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_AMMO_COUNT: {
                AmmoAmountEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_SCORE: {
                ScoreEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
                break;
            }
            case EVENT_PLAYER_STATUS: {
                PlayerStatusEvent evt;
                evt.deserialize(&networkData[i]);
                eventHandlerLookup.find(evt.getOpcode())->handle(evt);
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

void GameClient::sendReloadRequestEvent() const {
    ReloadRequestEvent evt;
    sendMessage(evt);
}

bool GameClient::getIsConnectedToServer() const {
    return network.isConnected();
}
