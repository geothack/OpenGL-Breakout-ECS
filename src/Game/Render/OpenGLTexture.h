#pragma once

class OpenGLTexture
{
public:
	OpenGLTexture() = default;
	OpenGLTexture(const std::filesystem::path& path, bool flip = false);

	void Attach();

	inline uint32_t const GetHandle() { return mHandle; }

private:
	uint32_t mHandle;

	int mWidth;
	int mHeight;
	int mChannels;
};