#include "texture.h"
// 引入stbImage库(需要先使用宏定义)
#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

Texture::Texture(const std::string& path, unsigned int unit) {
	mUnit = unit;

	// 1.stbImage读取图片
	int channels;
	// 翻转Y轴
	stbi_set_flip_vertically_on_load(true);
	// 读取出来的图片格式转化为RGBA
	unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	// 2.生成纹理对象,并且激活单元绑定
	glGenTextures(1, &mTexture);
	// 激活0号纹理单元,在openGL中激活了0号单元后,无法关掉该激活状态,只能去激活其他纹理单元***
	glActiveTexture(GL_TEXTURE0 + mUnit);
	// 绑定纹理对象,同时也将该纹理对象和0号纹理单元进行了绑定
	glBindTexture(GL_TEXTURE_2D, mTexture);

	// 3.传输纹理数据,同时会开启显存 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// **释放数据
	stbi_image_free(data);

	// 4.设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 5.设置纹理包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //t

	return;
}


Texture::~Texture() {
	if (mTexture != 0) {
		glDeleteTextures(1, &mTexture);
	}
	return;
}

void Texture::bind() {
	// 先切换纹理对象,然后绑定texture对象
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	return;
}