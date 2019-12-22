// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Erewhon Client" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <ClientLib/Scripting/Sound.hpp>
#include <stdexcept>

namespace bw
{
	void Sound::Stop()
	{
		if (!m_sound)
			throw std::runtime_error("Invalid sound");

		m_sound->StopSound(m_soundIndex);
	}
}