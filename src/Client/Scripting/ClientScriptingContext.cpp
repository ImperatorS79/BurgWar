// Copyright (C) 2018 Jérôme Leclercq
// This file is part of the "Burgwar Shared" project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Client/Scripting/ClientScriptingContext.hpp>
#include <Shared/Utils.hpp>
#include <iostream>

namespace bw
{
	ClientScriptingContext::ClientScriptingContext(LocalMatch& match, std::shared_ptr<VirtualDirectory> scriptDir) :
	SharedScriptingContext(false),
	m_scriptDirectory(std::move(scriptDir)),
	m_match(match)
	{
		Nz::LuaState& state = GetLuaInstance();
		state.PushFunction([&](Nz::LuaState& /*state*/) -> int
		{
			return 0;
		});
		state.SetGlobal("RegisterClientScript");

		RegisterLibrary();
	}

	bool ClientScriptingContext::Load(const std::filesystem::path& folderOrFile)
	{
		VirtualDirectory::Entry entry;
		if (!m_scriptDirectory->GetEntry(folderOrFile.generic_u8string(), &entry))
		{
			std::cerr << "Unknown path " << folderOrFile.generic_u8string() << std::endl;
			return false;
		}

		return std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, VirtualDirectory::DirectoryEntry>)
			{
				arg->Foreach([&](const std::string& entryName, VirtualDirectory::Entry)
				{
					Load(folderOrFile / entryName);
				});

				return true;
			}
			else if constexpr (std::is_same_v<T, VirtualDirectory::FileEntry>)
			{
				m_currentFolder = folderOrFile.parent_path();
				Nz::LuaInstance& state = GetLuaInstance();
				if (state.ExecuteFromMemory(arg.data(), arg.size()))
				{
					std::cout << "Loaded " << folderOrFile << std::endl;
					return true;
				}
				else
				{
					std::cerr << "Failed to load " << folderOrFile.generic_u8string() << ": " << state.GetLastError() << std::endl;
					return false;
				}
			}
			else
				static_assert(AlwaysFalse<T>::value, "non-exhaustive visitor");

		}, entry);
	}
}
