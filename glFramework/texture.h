#pragma once
#include "core.h"
#include<string>

class Texture {
public:
	Texture(const std::string& path, unsigned int unit);
	~Texture();

	// 绑定这个纹理为当前纹理对象
	void bind();

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 }; // 纹理单元


};
