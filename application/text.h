#pragma once
#include "../glFramework/core.h"
#include <map>
#include <iostream>
#include <string>
#include "../glFramework/shader.h"

struct Character
{
	GLuint     TextureID;  // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
};

class Text {
public:
	Text(int vaoID); // vaoID��ʾ���������ݷ���vao�ļ��Ż�����
	~Text();
	// x,y,z��ʾλ������,color��ʾ��ɫ
	void RenderText(Shader& s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int unit);
	void loadText(const char* path,int unit);

private:
	std::map<GLchar, Character> mCharacters;
	GLuint mTextVao{ 0 };
	GLuint mTextVbo{ 0 };

};