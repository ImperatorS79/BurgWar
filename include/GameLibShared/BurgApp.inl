// Copyright (C) 2019 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <GameLibShared/AnimationStore.hpp>
#include <cassert>
#include <stdexcept>
#include "BurgApp.hpp"

namespace bw
{
	inline Nz::UInt64 bw::BurgApp::GetAppTime() const
	{
		return m_appTime;
	}
}