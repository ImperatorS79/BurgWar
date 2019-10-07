// Copyright (C) 2019 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_CORELIB_LOGSYSTEM_LOGSINK_HPP
#define BURGWAR_CORELIB_LOGSYSTEM_LOGSINK_HPP

#include <string_view>

namespace bw
{
	template<typename Context>
	class LogSink
	{
		public:
			LogSink() = default;
			virtual ~LogSink() = default;

			virtual void Write(const Context& context, std::string_view content) = 0;
	};
}

#endif