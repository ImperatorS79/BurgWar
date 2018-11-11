// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Burgwar Shared" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EREWHON_SHARED_NETWORK_PACKETS_HPP
#define EREWHON_SHARED_NETWORK_PACKETS_HPP

#include <Shared/InputData.hpp>
#include <Shared/Protocol/CompressedInteger.hpp>
#include <Shared/Protocol/PacketSerializer.hpp>
#include <Nazara/Prerequisites.hpp>
#include <Nazara/Core/Color.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Angle.hpp>
#include <Nazara/Math/Box.hpp>
#include <Nazara/Math/Quaternion.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Network/NetPacket.hpp>
#include <array>
#include <optional>
#include <variant>
#include <vector>

namespace bw
{
	enum class PacketType
	{
		Auth,
		AuthFailure,
		AuthSuccess,
		CreateEntities,
		DeleteEntities,
		HelloWorld,
		MatchData,
		MatchState,
		NetworkStrings,
		PlayersInput
	};

	template<PacketType PT> struct PacketTag
	{
		static constexpr PacketType Type = PT;
	};

	namespace Packets
	{
#define DeclarePacket(Type) struct Type : PacketTag<PacketType:: Type >

		DeclarePacket(Auth)
		{
			Nz::UInt8 playerCount;
		};

		DeclarePacket(AuthFailure)
		{
		};

		DeclarePacket(AuthSuccess)
		{
		};

		DeclarePacket(CreateEntities)
		{
			struct PlayerMovementData
			{
				bool isAirControlling;
				bool isFacingRight;
			};

			struct PhysicsProperties
			{
				Nz::RadianAnglef angularVelocity;
				Nz::Vector2f linearVelocity;
			};

			struct Entity
			{
				CompressedUnsigned<Nz::UInt32> id;
				CompressedUnsigned<Nz::UInt32> entityClass;
				Nz::RadianAnglef rotation;
				Nz::Vector2f position;
				std::optional<CompressedUnsigned<Nz::UInt32>> parentId;
				std::optional<PlayerMovementData> playerMovement;
				std::optional<PhysicsProperties> physicsProperties;
			};

			std::vector<Entity> entities;
		};

		DeclarePacket(DeleteEntities)
		{
			struct Entity
			{
				CompressedUnsigned<Nz::UInt32> id;
			};

			std::vector<Entity> entities;
		};

		DeclarePacket(HelloWorld)
		{
			std::string str;
		};

		DeclarePacket(MatchData)
		{
			struct Layer
			{
				Nz::UInt16 height;
				Nz::UInt16 width;
				std::vector<Nz::UInt8> tiles; //< 0 = empty, 1 = dirt, 2 = dirt w/ grass
			};

			std::vector<Layer> layers;
			Nz::Color backgroundColor;
			float tileSize;
		};

		DeclarePacket(MatchState)
		{
			struct PlayerMovementData
			{
				bool isAirControlling;
				bool isFacingRight;
			};

			struct PhysicsProperties
			{
				Nz::RadianAnglef angularVelocity;
				Nz::Vector2f linearVelocity;
			};

			struct Entity
			{
				CompressedUnsigned<Nz::UInt32> id;
				Nz::RadianAnglef rotation;
				Nz::Vector2f position;
				std::optional<PlayerMovementData> playerMovement;
				std::optional<PhysicsProperties> physicsProperties;
			};

			std::vector<Entity> entities;
		};

		DeclarePacket(NetworkStrings)
		{
			CompressedUnsigned<Nz::UInt32> startId;
			std::vector<std::string> strings;
		};

		DeclarePacket(PlayersInput)
		{
			std::vector<std::optional<InputData>> inputs;
		};

#undef DeclarePacket

		void Serialize(PacketSerializer& serializer, Auth& data);
		void Serialize(PacketSerializer& serializer, AuthFailure& data);
		void Serialize(PacketSerializer& serializer, AuthSuccess& data);
		void Serialize(PacketSerializer& serializer, CreateEntities& data);
		void Serialize(PacketSerializer& serializer, DeleteEntities& data);
		void Serialize(PacketSerializer& serializer, HelloWorld& data);
		void Serialize(PacketSerializer& serializer, MatchData& data);
		void Serialize(PacketSerializer& serializer, MatchState& data);
		void Serialize(PacketSerializer& serializer, NetworkStrings& data);
		void Serialize(PacketSerializer& serializer, PlayersInput& data);
	}
}

#include <Shared/Protocol/Packets.inl>

#endif // EREWHON_SHARED_NETWORK_PACKETS_HPP
