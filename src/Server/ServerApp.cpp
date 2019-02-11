// Copyright (C) 2019 J�r�me Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Server/ServerApp.hpp>
#include <GameLibShared/NetworkSessionManager.hpp>
#include <Nazara/Core/Thread.hpp>
#include <iostream>

namespace bw
{
	ServerApp::ServerApp(int argc, char* argv[]) :
	Application(argc, argv)
	{
		m_match = std::make_unique<Match>(*this, "Je suis un match sur le serveur", "test", 10);
		m_match->GetSessions().CreateSessionManager<NetworkSessionManager>(14768, 10);
	}

	int ServerApp::Run()
	{
		while (Application::Run())
		{
			BurgApp::Update();

			m_match->Update(GetUpdateTime());

			//TODO: Sleep only when server is not overloaded
			Nz::Thread::Sleep(1);
		}

		return 0;
	}
}
