// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Burgwar Client" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Client/TerrainLayer.hpp>

namespace bw
{
	inline Ndk::World& TerrainLayer::GetWorld()
	{
		return m_world;
	}
}