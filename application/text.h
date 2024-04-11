#pragma once
#include "../glFramework/core.h"
#include <map>
#include <iostream>
#include <string>
#include "../glFramework/shader.h"

struct Character
{
	GLuint     TextureID;  // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;    // 原点距下一个字形原点的距离
};

class Text {
public:
	Text(int vaoID); // vaoID表示将文字数据放在vao的几号缓存中
	~Text();
	// x,y,z表示位置坐标,color表示颜色
	void RenderText(Shader& s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int unit);
	void loadText(const char* path,int unit);

private:
	std::map<GLchar, Character> mCharacters;
	GLuint mTextVao{ 0 };
	GLuint mTextVbo{ 0 };

};