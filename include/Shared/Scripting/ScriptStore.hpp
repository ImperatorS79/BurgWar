// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Burgwar Client" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_SHARED_SCRIPTING_SCRIPTSTORE_HPP
#define BURGWAR_SHARED_SCRIPTING_SCRIPTSTORE_HPP

#include <Nazara/Lua/LuaState.hpp>
#include <hopstotch/hopscotch_map.h>
#include <limits>
#include <vector>

namespace bw
{
	template<typename Element, bool IsServer>
	class ScriptStore
	{
		public:
			inline ScriptStore(Nz::LuaState& state);
			virtual ~ScriptStore() = default;

			template<typename F> void ForEachElement(const F& func) const;

			inline const Element& GetElement(std::size_t index) const;
			inline std::size_t GetElementIndex(const std::string& name) const;

			bool Load(const std::string& folder);

			static constexpr std::size_t InvalidIndex = std::numeric_limits<std::size_t>::max();

		protected:
			virtual void InitializeElementTable(Nz::LuaState& state) = 0;
			virtual void InitializeElement(Nz::LuaState& state, Element& element) = 0;

			Nz::LuaState& GetState();
			void SetElementTypeName(std::string typeName);
			void SetTableName(std::string tableName);

		private:
			std::string m_elementTypeName;
			std::string m_tableName;
			std::vector<Element> m_elements;
			tsl::hopscotch_map<std::string /*name*/, std::size_t /*elementIndex*/> m_elementsByName;
			Nz::LuaState& m_state;
	};
}

#include <Shared/Scripting/ScriptStore.inl>

#endif
