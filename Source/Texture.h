#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& path);
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

	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
};
