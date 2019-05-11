// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Erewhon Client" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_STATES_STATES_TIMESYNCSTATE_HPP
#define BURGWAR_STATES_STATES_TIMESYNCSTATE_HPP

#include <Client/States/AbstractState.hpp>
#include <ClientLib/ClientSession.hpp>
#include <CoreLib/Protocol/Packets.hpp>
#include <Nazara/Core/Clock.hpp>
#include <Nazara/Graphics/TextSprite.hpp>
#include <Nazara/Renderer/RenderTarget.hpp>
#include <NDK/EntityOwner.hpp>
#include <NDK/State.hpp>
#include <NDK/World.hpp>
#include <vector>

namespace bw
{
	class TimeSyncState final : public AbstractState
	{
		public:
			inline TimeSyncState(std::shared_ptr<StateData> stateData, std::shared_ptr<ClientSession> clientSession, std::shared_ptr<AbstractState> nextState);
			~TimeSyncState() = default;

		private:
			void Enter(Ndk::StateMachine& fsm) override;
			void Leave(Ndk::StateMachine& fsm) override;
			bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

			void LayoutWidgets() override;

			void OnTimeSyncResponse(ClientSession* session, const Packets::TimeSyncResponse& response);
			void UpdateStatus(const Nz::String& status, const Nz::Color& color = Nz::Color::White);

			std::shared_ptr<AbstractState> m_nextState;
			std::shared_ptr<ClientSession> m_clientSession;
			std::vector<Nz::UInt64> m_results;
			Ndk::EntityOwner m_statusText;
			Nz::TextSpriteRef m_statusSprite;
			Nz::UInt8 m_expectedRequestId;
			Nz::UInt64 m_pingAccumulator;
			Nz::UInt64 m_requestTime;
			bool m_finished;
			bool m_isClientYounger;
			float m_accumulator;
			float m_nextStepTime;
	};
}

#include <Client/States/Game/TimeSyncState.inl>

#endif
