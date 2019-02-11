// Copyright (C) 2019 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GameLibShared/Components/PropertiesComponent.hpp>

namespace bw
{
	inline PropertiesComponent::PropertiesComponent(Properties properties) :
	m_properties(std::move(properties))
	{
	}
}
