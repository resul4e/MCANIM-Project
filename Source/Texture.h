#pragma once
#include <string>
#include <filesystem>
class Texture
{
public:
	Texture(std::filesystem::path _filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int  GetWidth() const { return m_Width; }
	inline int	GetHeight() const { return m_Height; }
	inline int	GetBytePerPixel() const { return m_BPP; }

private:
	unsigned int m_rendererID;

	int id;
	int m_Width;
	int m_Height;
	int m_BPP;

	float offsetX;
	float offsetY;

	std::filesystem::path m_FilePath;
	unsigned char* m_LocalBuffer;

	friend class Renderer;
};
