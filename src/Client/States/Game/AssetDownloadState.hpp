// Copyright (C) 2020 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_STATES_GAME_ASSETDOWNLOADSTATE_HPP
#define BURGWAR_STATES_GAME_ASSETDOWNLOADSTATE_HPP

#include <ClientLib/HttpDownloadManager.hpp>
#include <Client/States/Game/StatusState.hpp>
#include <NDK/Widgets/LabelWidget.hpp>
#include <optional>

namespace bw
{
	class ClientSession;

	class AssetDownloadState final : public StatusState
	{
		public:
			AssetDownloadState(std::shared_ptr<StateData> stateData, std::shared_ptr<ClientSession> clientSession, Packets::AuthSuccess authSuccess, Packets::MatchData matchData);
			~AssetDownloadState() = default;

		private:
			void Enter(Ndk::StateMachine& fsm) override;
			bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

			std::optional<HttpDownloadManager> m_httpDownloadManager;
			std::shared_ptr<AbstractState> m_nextState;
			std::shared_ptr<ClientSession> m_clientSession;
			Packets::AuthSuccess m_authSuccess;
			Packets::MatchData m_matchData;
			float m_nextStateDelay;
	};
}

#include <Client/States/Game/AssetDownloadState.inl>

#endif
