#include "texture.h"
// ����stbImage��(��Ҫ��ʹ�ú궨��)
#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

Texture::Texture(const std::string& path, unsigned int unit) {
	mUnit = unit;

	// 1.stbImage��ȡͼƬ
	int channels;
	// ��תY��
	stbi_set_flip_vertically_on_load(true);
	// ��ȡ������ͼƬ��ʽת��ΪRGBA
	unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	// 2.�����������,���Ҽ��Ԫ��
	glGenTextures(1, &mTexture);
	// ����0������Ԫ,��openGL�м�����0�ŵ�Ԫ��,�޷��ص��ü���״̬,ֻ��ȥ������������Ԫ***
	glActiveTexture(GL_TEXTURE0 + mUnit);
	// ���������,ͬʱҲ������������0������Ԫ�����˰�
	glBindTexture(GL_TEXTURE_2D, mTexture);

	// 3.������������,ͬʱ�Ὺ���Դ� 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// **�ͷ�����
	stbi_image_free(data);

	// 4.����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 5.�������������ʽ
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
	// ���л��������,Ȼ���texture����
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	return;
}