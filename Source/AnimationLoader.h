#pragma once
#include <filesystem>

class AnimationLoader
{
public:
	AnimationLoader() = delete;
	~AnimationLoader() = delete;

	static void LoadAnimation(std::filesystem::path _filePath);

private:

};