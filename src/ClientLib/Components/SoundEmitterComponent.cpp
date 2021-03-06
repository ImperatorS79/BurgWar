// Copyright (C) 2020 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <ClientLib/Components/SoundEmitterComponent.hpp>
#include <NDK/World.hpp>
#include <ClientLib/Systems/SoundSystem.hpp>

namespace bw
{
	Nz::UInt32 SoundEmitterComponent::PlaySound(const Nz::SoundBufferRef& soundBuffer, const Nz::Vector3f& soundPosition, bool attachedToEntity, bool isLooping, bool isSpatialized)
	{
		const Ndk::EntityHandle& entity = GetEntity();
		if (!entity)
			return SoundSystem::InvalidSoundId;

		auto& soundSystem = entity->GetWorld()->GetSystem<SoundSystem>();
		if (Nz::UInt32 soundId = soundSystem.PlaySound(soundBuffer, soundPosition, attachedToEntity, isLooping, isSpatialized); soundId != SoundSystem::InvalidSoundId)
		{
			m_sounds.insert(soundId);
			return soundId;
		}
		else
			return SoundSystem::InvalidSoundId;
	}

	void SoundEmitterComponent::StopSound(Nz::UInt32 soundId)
	{
		auto it = m_sounds.find(soundId);
		if (it == m_sounds.end())
			throw std::runtime_error("This sound doesn't belong to this entity");

		const Ndk::EntityHandle& entity = GetEntity();
		if (entity)
		{
			auto& soundSystem = entity->GetWorld()->GetSystem<SoundSystem>();
			soundSystem.StopSound(soundId);
		}

		m_sounds.erase(it);
	}

	Ndk::ComponentIndex SoundEmitterComponent::componentIndex;
}
