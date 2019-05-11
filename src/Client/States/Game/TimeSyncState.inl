// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Erewhon Client" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Client/States/Game/TimeSyncState.hpp>

namespace bw
{
	inline TimeSyncState::TimeSyncState(std::shared_ptr<StateData> stateData, std::shared_ptr<ClientSession> clientSession, std::shared_ptr<AbstractState> nextState) :
	AbstractState(std::move(stateData)),
	m_nextState(std::move(nextState)),
	m_clientSession(std::move(clientSession))
	{
	}
}
