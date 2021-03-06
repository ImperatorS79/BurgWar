// Copyright (C) 2020 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_CORELIB_MATCH_HPP
#define BURGWAR_CORELIB_MATCH_HPP

#include <Nazara/Core/Bitset.hpp>
#include <Nazara/Core/ByteArray.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include <Nazara/Network/UdpSocket.hpp>
#include <CoreLib/AssetStore.hpp>
#include <CoreLib/Map.hpp>
#include <CoreLib/MatchSessions.hpp>
#include <CoreLib/Player.hpp>
#include <CoreLib/SharedMatch.hpp>
#include <CoreLib/TerrainLayer.hpp>
#include <CoreLib/LogSystem/MatchLogger.hpp>
#include <CoreLib/Protocol/NetworkStringStore.hpp>
#include <CoreLib/Scripting/ScriptingContext.hpp>
#include <CoreLib/Scripting/ServerEntityStore.hpp>
#include <CoreLib/Scripting/ServerWeaponStore.hpp>
#include <Thirdparty/tsl/hopscotch_map.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace bw
{
	class BurgApp;
	class ServerGamemode;
	class ServerScriptingLibrary;
	class Terrain;

	enum class DisconnectionReason
	{
		Kicked,
		PlayerLeft,
		TimedOut
	};

	class Match : public SharedMatch
	{
		friend class MatchClientSession;

		public:
			struct Asset;
			struct ClientScript;

			Match(BurgApp& app, std::string matchName, std::filesystem::path gamemodeFolder, Map map, std::size_t maxPlayerCount, float tickDuration);
			Match(const Match&) = delete;
			Match(Match&&) = delete;
			~Match();

			inline Nz::Int64 AllocateUniqueId();

			template<typename T> void BroadcastPacket(const T& packet, bool onlyReady = true);

			template<typename T> void BuildClientAssetListPacket(T& clientAsset) const;
			template<typename T> void BuildClientScriptListPacket(T& clientScript) const;

			Player* CreatePlayer(MatchClientSession& session, Nz::UInt8 localIndex, std::string name);

			void ForEachEntity(std::function<void(const Ndk::EntityHandle& entity)> func) override;
			template<typename F> void ForEachPlayer(F&& func);

			inline BurgApp& GetApp();
			inline AssetStore& GetAssetStore();
			bool GetClientScript(const std::string& filePath, const ClientScript** clientScriptData);
			ServerEntityStore& GetEntityStore() override;
			const ServerEntityStore& GetEntityStore() const override;
			inline const std::shared_ptr<ServerGamemode>& GetGamemode();
			inline const std::filesystem::path& GetGamemodePath() const;
			TerrainLayer& GetLayer(LayerIndex layerIndex) override;
			const TerrainLayer& GetLayer(LayerIndex layerIndex) const override;
			LayerIndex GetLayerCount() const override;
			inline sol::state& GetLuaState();
			inline const Packets::MatchData& GetMatchData() const;
			const NetworkStringStore& GetNetworkStringStore() const override;
			inline MatchSessions& GetSessions();
			inline const MatchSessions& GetSessions() const;
			inline const std::shared_ptr<ServerScriptingLibrary>& GetScriptingLibrary() const;
			inline Terrain& GetTerrain();
			inline const Terrain& GetTerrain() const;
			ServerWeaponStore& GetWeaponStore() override;
			const ServerWeaponStore& GetWeaponStore() const override;

			void InitDebugGhosts();

			void RegisterAsset(const std::filesystem::path& assetPath);
			void RegisterAsset(std::string assetPath, Nz::UInt64 assetSize, Nz::ByteArray assetChecksum);
			void RegisterClientScript(const std::filesystem::path& clientScript);
			void RegisterEntity(Nz::Int64 uniqueId, Ndk::EntityHandle entity);
			void RegisterNetworkString(std::string string);

			void ReloadAssets();
			void ReloadScripts();

			void RemovePlayer(Player* player, DisconnectionReason disconnection);

			const Ndk::EntityHandle& RetrieveEntityByUniqueId(Nz::Int64 uniqueId) const override;
			Nz::Int64 RetrieveUniqueIdByEntity(const Ndk::EntityHandle& entity) const override;

			void Update(float elapsedTime);

			Match& operator=(const Match&) = delete;
			Match& operator=(Match&&) = delete;

			struct Asset
			{
				Nz::ByteArray checksum;
				Nz::UInt64 size;
				std::string path;
			};

			struct ClientScript
			{
				Nz::ByteArray checksum;
				std::vector<Nz::UInt8> content;
			};

		private:
			void BuildMatchData();
			void OnPlayerReady(Player* player);
			void OnTick(bool lastTick) override;
			void SendPingUpdate();

			struct Debug
			{
				Nz::UdpSocket socket;
				Nz::UInt64 lastBroadcastTime = 0;
			};

			struct Entity
			{
				Ndk::EntityHandle entity;

				NazaraSlot(Ndk::Entity, OnEntityDestruction, onDestruction);
			};

			std::filesystem::path m_gamemodePath;
			std::optional<AssetStore> m_assetStore;
			std::optional<Debug> m_debug;
			std::optional<ServerEntityStore> m_entityStore;
			std::optional<ServerWeaponStore> m_weaponStore;
			std::size_t m_maxPlayerCount;
			std::shared_ptr<ServerGamemode> m_gamemode;
			std::shared_ptr<ScriptingContext> m_scriptingContext;
			std::shared_ptr<ServerScriptingLibrary> m_scriptingLibrary;
			std::string m_name;
			std::unique_ptr<Terrain> m_terrain;
			std::vector<std::unique_ptr<Player>> m_players;
			mutable Packets::MatchData m_matchData;
			tsl::hopscotch_map<std::string, Asset> m_assets;
			tsl::hopscotch_map<std::string, ClientScript> m_clientScripts;
			tsl::hopscotch_map<Nz::Int64, Entity> m_entitiesByUniqueId;
			Nz::Bitset<> m_freePlayerId;
			Nz::Int64 m_nextUniqueId;
			Nz::UInt64 m_lastPingUpdate;
			BurgApp& m_app;
			Map m_map;
			MatchSessions m_sessions;
			NetworkStringStore m_networkStringStore;
			bool m_disableWhenEmpty;
	};
}

#include <CoreLib/Match.inl>

#endif
