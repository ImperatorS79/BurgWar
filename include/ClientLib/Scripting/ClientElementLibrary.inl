// Copyright (C) 2020 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <ClientLib/Scripting/ClientElementLibrary.hpp>
#include <cassert>

namespace bw
{
	inline ClientElementLibrary::ClientElementLibrary(const Logger& logger, ClientAssetStore& assetStore) :
	SharedElementLibrary(logger),
	m_assetStore(assetStore)
	{
	}
}