// Copyright (C) 2019 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <CoreLib/TerrainLayer.hpp>
#include <CoreLib/Match.hpp>
#include <CoreLib/Components/NetworkSyncComponent.hpp>
#include <CoreLib/Components/PlayerControlledComponent.hpp>
#include <CoreLib/Components/PlayerMovementComponent.hpp>
#include <CoreLib/Systems/AnimationSystem.hpp>
#include <CoreLib/Systems/NetworkSyncSystem.hpp>
#include <CoreLib/Systems/PlayerMovementSystem.hpp>
#include <CoreLib/Systems/TickCallbackSystem.hpp>
#include <Nazara/Physics2D/Arbiter2D.hpp>
#include <NDK/Components.hpp>
#include <NDK/Systems.hpp>
#include <iostream>

namespace bw
{
	TerrainLayer::TerrainLayer(BurgApp& app, Match& match, const Map::Layer& layerData) :
	m_world(app, match)
	{
		Ndk::World& world = m_world.GetWorld();
		world.AddSystem<NetworkSyncSystem>();

		auto& entityStore = match.GetEntityStore();
		for (const Map::Entity& entityData : layerData.entities)
		{
			std::size_t entityTypeIndex = entityStore.GetElementIndex(entityData.entityType);
			if (entityTypeIndex == entityStore.InvalidIndex)
			{
				std::cerr << "Unknown entity type " + entityData.entityType << std::endl;
				continue;
			}

			entityStore.InstantiateEntity(world, entityTypeIndex, entityData.position, entityData.rotation, entityData.properties);
		}
	}

	void TerrainLayer::Update(float elapsedTime)
	{
		Ndk::World& world = m_world.GetWorld();
		world.Update(elapsedTime);
	}
}
