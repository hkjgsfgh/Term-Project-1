#include "text.h"

Text::Text(int vaoID) {
	glGenVertexArrays(1, &mTextVao);
	glGenBuffers(1, &mTextVbo);
	glBindVertexArray(mTextVao);
	glBindBuffer(GL_ARRAY_BUFFER, mTextVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(vaoID);
	glVertexAttribPointer(vaoID, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return;
}

Text::~Text() {

}


void Text::RenderText(Shader& s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color,int unit) {
	// �����Ӧ����Ⱦ״̬
	s.begin();
	s.setVector3("textColor", color.x, color.y, color.z);
	s.setInt("text", unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindVertexArray(mTextVao);

	// �����ı������е��ַ�
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = mCharacters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// ��ÿ���ַ�����VBO
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// ���ı����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// ����VBO�ڴ������
		glBindBuffer(GL_ARRAY_BUFFER, mTextVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// �����ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	s.end();

	return;
}

void Text::loadText(const char* path,int unit) {
	FT_Library library;
	if (FT_Init_FreeType(&library)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(library, path, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������

	for (GLubyte ch = 0; ch < 128; ch++)
	{
		if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		mCharacters.insert(std::pair<GLchar, Character>(ch, character));
	}

	// ������Ԫ
	//glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(library);


	return;
}
