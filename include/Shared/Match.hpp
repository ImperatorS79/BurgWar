// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Burgwar Client" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_SHARED_MATCH_HPP
#define BURGWAR_SHARED_MATCH_HPP

#include <Nazara/Core/ObjectHandle.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include <Shared/MatchSessions.hpp>
#include <Shared/Protocol/NetworkStringStore.hpp>
#include <Shared/Scripting/ServerEntityStore.hpp>
#include <Shared/Scripting/ServerWeaponStore.hpp>
#include <memory>
#include <string>
#include <vector>

namespace bw
{
	class Player;
	class Terrain;

	using PlayerHandle = Nz::ObjectHandle<Player>;

	class Match
	{
		public:
			Match(std::string matchName, std::size_t maxPlayerCount);
			Match(const Match&) = delete;
			~Match();

			void Leave(Player* player);

			inline ServerEntityStore& GetEntityStore();
			inline const ServerEntityStore& GetEntityStore() const;
			inline Nz::LuaInstance& GetLuaInstance();
			inline const NetworkStringStore& GetNetworkStringStore() const;
			inline MatchSessions& GetSessions();
			inline const MatchSessions& GetSessions() const;
			inline Terrain& GetTerrain();
			inline const Terrain& GetTerrain() const;
			inline ServerWeaponStore& GetWeaponStore();
			inline const ServerWeaponStore& GetWeaponStore() const;

			bool Join(Player* player);

			void Update(float elapsedTime);

			Match& operator=(const Match&) = delete;

		private:
			std::size_t m_maxPlayerCount;
			std::string m_name;
			std::unique_ptr<Terrain> m_terrain;
			std::vector<PlayerHandle> m_players;
			Nz::LuaInstance m_luaInstance;
			MatchSessions m_sessions;
			NetworkStringStore m_networkStringStore;
			ServerEntityStore m_entityStore;
			ServerWeaponStore m_weaponStore;
	};
}

#include <Shared/Match.inl>

#endif
