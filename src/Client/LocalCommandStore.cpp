// Copyright (C) 2019 J�r�me Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Client/LocalCommandStore.hpp>
#include <Client/ClientSession.hpp>
#include <GameLibShared/Protocol/Packets.hpp>

namespace bw
{
	LocalCommandStore::LocalCommandStore()
	{
#define IncomingCommand(Type) RegisterIncomingCommand<Packets::Type>(#Type, [](ClientSession* session, const Packets::Type& packet) \
{ \
	session->HandleIncomingPacket(packet); \
})
#define OutgoingCommand(Type, Flags, Channel) RegisterOutgoingCommand<Packets::Type>(#Type, Flags, Channel)

		// Incoming commands
		IncomingCommand(AuthFailure);
		IncomingCommand(AuthSuccess);
		IncomingCommand(ClientScriptList);
		IncomingCommand(ControlEntity);
		IncomingCommand(CreateEntities);
		IncomingCommand(DeleteEntities);
		IncomingCommand(DownloadClientScriptResponse);
		IncomingCommand(EntitiesInputs);
		IncomingCommand(HealthUpdate);
		IncomingCommand(HelloWorld);
		IncomingCommand(MatchData);
		IncomingCommand(MatchState);
		IncomingCommand(NetworkStrings);
		IncomingCommand(PlayAnimation);

		// Outgoing commands
		OutgoingCommand(Auth,                        Nz::ENetPacketFlag_Reliable, 0);
		OutgoingCommand(DownloadClientScriptRequest, Nz::ENetPacketFlag_Reliable, 0);
		OutgoingCommand(HelloWorld,                  Nz::ENetPacketFlag_Reliable, 0);
		OutgoingCommand(NetworkStrings,              Nz::ENetPacketFlag_Reliable, 0);
		OutgoingCommand(PlayersInput,                Nz::ENetPacketFlag_Reliable, 0);
		OutgoingCommand(Ready,                       Nz::ENetPacketFlag_Reliable, 0);

#undef IncomingCommand
#undef OutgoingCommand
	}
}
