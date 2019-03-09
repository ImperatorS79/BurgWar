// Copyright (C) 2019 Jérôme Leclercq
// This file is part of the "Burgwar" project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BURGWAR_MAPEDITOR_SCRIPTING_ENTITY_EDITOR_MODE_HPP
#define BURGWAR_MAPEDITOR_SCRIPTING_ENTITY_EDITOR_MODE_HPP

#include <MapEditor/Logic/EditorMode.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <NDK/Entity.hpp>
#include <vector>

namespace bw
{
	class EntityEditorMode : public EditorMode
	{
		public:
			EntityEditorMode(const Ndk::EntityHandle& targetEntity);
			~EntityEditorMode() = default;

			void OnEnter(EditorWindow& editor) override;
			void OnLeave(EditorWindow& editor) override;

		private:
			struct Renderable
			{
				std::vector<Nz::MaterialRef> originalMaterials;
			};

			struct Entity
			{
				Ndk::EntityHandle entity;
				std::vector<Renderable> renderables;
			};

			Ndk::EntityHandle m_targetEntity;
			std::vector<Entity> m_entities;
	};
}

#include <MapEditor/Logic/EntityEditorMode.inl>

#endif
