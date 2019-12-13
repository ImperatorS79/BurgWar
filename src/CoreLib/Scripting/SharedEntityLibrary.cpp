// Copyright (C) 2019 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <CoreLib/Scripting/SharedEntityLibrary.hpp>
#include <CoreLib/PlayerMovementController.hpp>
#include <CoreLib/Components/HealthComponent.hpp>
#include <CoreLib/Components/PlayerMovementComponent.hpp>
#include <CoreLib/Scripting/AbstractScriptingLibrary.hpp> // For sol metainfo
#include <CoreLib/Scripting/SharedScriptingLibrary.hpp> // For sol metainfo
#include <Nazara/Core/CallOnExit.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <sol3/sol.hpp>

namespace bw
{
	void SharedEntityLibrary::RegisterLibrary(sol::table& elementMetatable)
	{
		RegisterSharedLibrary(elementMetatable);
	}

	void SharedEntityLibrary::InitRigidBody(const Ndk::EntityHandle& entity, float mass, float friction, bool canRotate)
	{
		auto& entityPhys = entity->AddComponent<Ndk::PhysicsComponent2D>();
		entityPhys.SetMass(mass);
		entityPhys.SetFriction(friction);

		if (!canRotate)
			entityPhys.SetMomentOfInertia(std::numeric_limits<float>::infinity());
	}

	void SharedEntityLibrary::RegisterSharedLibrary(sol::table& elementMetatable)
	{
		elementMetatable["ApplyImpulse"] = [this](const sol::table& entityTable, const Nz::Vector2f& force)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			if (entity->HasComponent<Ndk::PhysicsComponent2D>())
			{
				Ndk::PhysicsComponent2D& hitEntityPhys = entity->GetComponent<Ndk::PhysicsComponent2D>();
				hitEntityPhys.AddImpulse(force);
			}
		};

		elementMetatable["Damage"] = [](const sol::table& entityTable, Nz::UInt16 damage)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity->HasComponent<HealthComponent>())
				return;

			auto& entityHealth = entity->GetComponent<HealthComponent>();
			entityHealth.Damage(damage, Ndk::EntityHandle::InvalidHandle);
		};

		elementMetatable["EnableCollisionCallbacks"] = [](const sol::table& entityTable, bool enable)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity->HasComponent<Ndk::CollisionComponent2D>())
				throw std::runtime_error("Entity has no colliders");

			auto& collisionComponent = entity->GetComponent<Ndk::CollisionComponent2D>();

			// FIXME: For now, collision changes are only taken in account on SetGeom
			Nz::Collider2DRef geom = collisionComponent.GetGeom();
			geom->SetCollisionId((enable) ? 1 : 0);

			collisionComponent.SetGeom(std::move(geom));
		};

		elementMetatable["GetHealth"] = [](const sol::table& entityTable) -> Nz::UInt16
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity->HasComponent<HealthComponent>())
				return 0;

			auto& entityHealth = entity->GetComponent<HealthComponent>();
			return entityHealth.GetHealth();
		};

		elementMetatable["GetMass"] = [](const sol::table& entityTable)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			if (entity->HasComponent<Ndk::PhysicsComponent2D>())
			{
				auto& physComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();
				return physComponent.GetMass();
			}
			else
				return 0.f;
		};

		elementMetatable["GetPlayerMovementController"] = [](const sol::table& entityTable)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			if (!entity->HasComponent<PlayerMovementComponent>())
				throw std::runtime_error("Entity has no player movement");

			return entity->GetComponent<PlayerMovementComponent>().GetController();
		};

		elementMetatable["Heal"] = [](const sol::table& entityTable, Nz::UInt16 value)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity->HasComponent<HealthComponent>())
				return;

			auto& entityHealth = entity->GetComponent<HealthComponent>();
			entityHealth.Heal(value);
		};

		elementMetatable["IsFullHealth"] = [](const sol::table& entityTable) -> bool
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity->HasComponent<HealthComponent>())
				return false;

			auto& entityHealth = entity->GetComponent<HealthComponent>();
			return entityHealth.GetHealth() >= entityHealth.GetMaxHealth();
		};

		auto InitRigidBody = [this](const sol::table& entityTable, float mass, float friction = 0.f, bool canRotate = true)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			this->InitRigidBody(entity, mass, friction, canRotate);
		};

		elementMetatable["InitRigidBody"] = sol::overload(InitRigidBody,
			[=](const sol::table& entityTable, float mass, float friction) { InitRigidBody(entityTable, mass, friction); },
			[=](const sol::table& entityTable, float mass) { InitRigidBody(entityTable, mass); });

		elementMetatable["IsPlayerOnGround"] = [](const sol::table& entityTable)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			if (!entity->HasComponent<PlayerMovementComponent>())
				throw std::runtime_error("Entity has no player movement");

			return entity->GetComponent<PlayerMovementComponent>().IsOnGround();
		};

		elementMetatable["Kill"] = [](const sol::table& entityTable)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (entity->HasComponent<HealthComponent>())
			{
				auto& entityHealth = entity->GetComponent<HealthComponent>();
				entityHealth.Damage(entityHealth.GetHealth(), entity);
			}
			else
				entity->Kill();
		};

		elementMetatable["Remove"] = [](const sol::table& entityTable)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			entity->Kill();
		};

		elementMetatable["SetCollider"] = [](sol::this_state L, const sol::table& entityTable, const sol::table& colliderTable, std::optional<bool> isTriggerOpt)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			auto ParseCollider = [&L](const sol::table& collider) -> Nz::Collider2DRef
			{
				sol::object metatableOpt = collider[sol::metatable_key];
				if (!metatableOpt)
					return nullptr;

				sol::table metatable = metatableOpt.as<sol::table>();

				std::string typeName = metatable["__name"];

				if (typeName == "rect")
				{
					Nz::Rectf rect = collider.as<Nz::Rectf>();
					return Nz::BoxCollider2D::New(rect);
				}
				else if (typeName == "circle")
				{
					Nz::Vector2f origin = collider["origin"];
					float radius = collider["radius"];

					return Nz::CircleCollider2D::New(radius, origin);
				}
				else if (typeName == "segment")
				{
					Nz::Vector2f first = collider["first"];
					Nz::Vector2f second = collider["second"];

					return Nz::SegmentCollider2D::New(first, second);
				}
				else
				{
					luaL_argerror(L, 2, ("Invalid collider type: " + typeName).c_str());
					return nullptr;
				}
			};

			Nz::Collider2DRef collider = ParseCollider(colliderTable);
			if (!collider)
			{
				std::size_t colliderCount = colliderTable.size();
				luaL_argcheck(L, colliderCount > 0, 2, "Invalid collider count");

				if (colliderCount == 1)
					collider = ParseCollider(colliderTable[1]);
				else
				{
					std::vector<Nz::Collider2DRef> colliders(colliderCount);
					for (std::size_t i = 0; i < colliderCount; ++i)
					{
						colliders[i] = ParseCollider(colliderTable[i + 1]);
						luaL_argcheck(L, colliders[i].IsValid(), 2, ("Invalid collider #" + std::to_string(i + 1)).c_str());
					}

					collider = Nz::CompoundCollider2D::New(std::move(colliders));
				}
			}

			collider->SetTrigger((isTriggerOpt) ? *isTriggerOpt : false);

			entity->AddComponent<Ndk::CollisionComponent2D>(collider);
		};

		elementMetatable["SetMass"] = [](const sol::table& entityTable, float mass)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity)
				return;

			if (entity->HasComponent<Ndk::PhysicsComponent2D>())
			{
				auto& physComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();
				physComponent.SetMass(mass, false);
			}
		};

		elementMetatable["SetPosition"] = [](const sol::table& entityTable, const Nz::Vector2f& position)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity)
				return;

			if (entity->HasComponent<Ndk::PhysicsComponent2D>())
			{
				auto& physComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();
				physComponent.SetPosition(position);
			}
			else
			{
				auto& nodeComponent = entity->GetComponent<Ndk::NodeComponent>();
				nodeComponent.SetPosition(position);
			}
		};

		elementMetatable["SetVelocity"] = [](const sol::table& entityTable, const Nz::Vector2f& velocity)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);
			if (!entity || !entity->HasComponent<Ndk::PhysicsComponent2D>())
				return;

			auto& physComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();
			physComponent.SetVelocity(velocity);
		};

		elementMetatable["UpdatePlayerMovementController"] = [](const sol::table& entityTable, std::shared_ptr<PlayerMovementController> controller)
		{
			const Ndk::EntityHandle& entity = AssertScriptEntity(entityTable);

			if (!entity->HasComponent<PlayerMovementComponent>())
				throw std::runtime_error("Entity has no player movement");

			return entity->GetComponent<PlayerMovementComponent>().UpdateController(std::move(controller));
		};
	}
}
