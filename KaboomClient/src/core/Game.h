#pragma once

#include <Camera.h>
#include <GeometryObjectManager.h>
#include <MaterialManager.h>
#include <ParticleEffectManager.h>
#include <core/DeathmatchMode.h>
#include <core/EntityManager.h>
#include <core/Player.h>
//#include <core/GameMode.h>
#include <util/ConfigSettings.h>
#include "CharacterFactory.h"
#include "BombFactory.h"
#include "JumpPadFactory.h"
#include "PickupFactory.h"
#include "../network/ClientEventHandlerLookup.h"
#include "../network/GameClient.h"
#include "../gui/GameGUIEventHandler.h"
#include "../gui/GameGUIListener.h"
#include "../sound/SoundManager.h"
#include "LibRocketGUIManager.h"
#include "GameStateMachine.h"
#include <components/PlayerStatusComponent.h>

class InputManager;
using namespace ours; 
	class Game {
	public:
		enum VoiceActing{
			END_GAME_DEFEAT_1,
			END_GAME_DEFEAT_2,
			END_GAME_DEFEAT_3,
			END_GAME_VICTORY_1,
			END_GAME_VICTORY_2,
			END_GAME_VICTORY_3,
			KILL_1,
			KILL_2,
			KILL_3,
			DEATH_1,
			DEATH_2,
			DEATH_3,
			CHAMP_SELECT,
		};
		typedef std::unordered_map<unsigned int, Player *> IdToPlayerMap;

		Game(ConfigSettings *config);
		~Game();

		void run();

		inline void addPlayer(Player *player) {
			players[player->getId()] = player;
		}

		inline void removePlayer(Player *player) {
			players.erase(players.find(player->getId()));

			delete player;
		}

		void addEntity(Entity *entity);
		void removeEntity(Entity *entity);
		void removeAllEntities();

		inline ours::SoundManager &getSoundManager() {
			return soundManager;
		}

		inline EntityManager &getEntityManager() {
			return entityManager;
		}

		inline const CharacterFactory &getCharacterFactory() const {
			return characterFactory;
		}

		inline const BombFactory &getBombFactory() const {
			return bombFactory;
		}

		inline const JumpPadFactory &getJumpPadFactory() const {
			return jumpPadFactory;
		}

		inline const PickupFactory &getPickupFactory() const {
			return pickupFactory;
		}

		inline const GameClient &getGameClient() const {
			return client;
		}

		inline GeometryObjectManager *getGeometryManager() {
			return _geometryManager;
		}

		inline MaterialManager *getMaterialManager() {
			return _materialManager;
		}

		inline ParticleEffectManager *getParticleEffectManager() {
			return _particleEffectManager;
		}

		inline const GameGUIEventHandler *getGameGUIEventHandler() const {
			return _guiEventHandler;
		}

		inline const GameStateMachine getCurrentGameState() const {
			return gsm;
		}

		inline Camera &getCamera() {
			return _camera;
		}

		inline Player *getCurrentPlayer() const {
			return currentPlayer;
		}

		inline void setCurrentPlayer(Player *currentPlayer) {
			this->currentPlayer = currentPlayer;
		}

		inline const IdToPlayerMap &getPlayers() const {
			return players;
		}

		inline DeathmatchMode &getGameMode() {
			return gameMode;
		}
		void addVoiceLines(std::string str_mediaPath, int i, std::unordered_map<VoiceActing, osg::ref_ptr<Sample>> *voice);
		void damageScreenCheck();
		void deathTimeUpdate();

		std::string *name;
		unsigned int colorId=0;
		std::chrono::high_resolution_clock::time_point damageTime;
		std::chrono::high_resolution_clock::time_point deathTime;
		bool healthChanged = false;
		bool playerAlive = true;
		unsigned int previousValue=5;
		osg::ref_ptr<Source> voiceSource;
		std::unordered_map<VoiceActing, osg::ref_ptr<Sample>> *voiceMap;
		std::unordered_map<VoiceActing, osg::ref_ptr<Sample>>* voiceActorList[4];

	private:
		friend void GameGUIListener::setGameState(GameStateMachine state);

		GameStateMachine gsm = EDITOR_MODE;
		ConfigSettings *config;
		InputManager *inputManager;
		ours::SoundManager soundManager;

		EntityManager entityManager;
		CharacterFactory characterFactory;
		BombFactory bombFactory;
		JumpPadFactory jumpPadFactory;
		PickupFactory pickupFactory;

		ClientEventHandlerLookup eventHandlerLookup;
		GameClient client;

		Player *currentPlayer;
		IdToPlayerMap players;

		GeometryObjectManager *_geometryManager;
		MaterialManager *_materialManager;
		ParticleEffectManager *_particleEffectManager;

		Camera &_camera;
		GameGUIEventHandler *_guiEventHandler;

		bool abc;
		osg::ref_ptr<Source> backGroundMusic;

		osg::ref_ptr<Sample> angryRobot;
		bool angry;
		int color = 255;
		bool changeColor = true;
		
		DeathmatchMode gameMode;
	};