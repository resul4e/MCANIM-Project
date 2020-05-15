#pragma once
#include <memory>
#include <filesystem>

class Rig;

class RigLoader
{
public:
	RigLoader() = delete;
	~RigLoader() = delete;

	static std::shared_ptr<Rig> LoadRig(std::filesystem::path _filePath);
};