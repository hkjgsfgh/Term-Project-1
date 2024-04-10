#pragma once
#include "core.h"
#include<string>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void begin(); // ��ʼʹ�õ�ǰ��shader
	void end(); // ����ʹ�õ�ǰ��shader
	
	void setFloat(const std::string& name,float value);
	void setVector3(const std::string& name, float x, float y, float z);
	void setVector3(const std::string& name, int size, const float* values);
	void setInt(const std::string& name, int value);
	void setMatrix4x4(const std::string& name, glm::mat4 value);

private:
	// ��������,shader/program; type������COMPILE����LINK
	void checkShaderErrors(GLuint target, std::string type);

private:
	GLuint mProgram{ 0 };
};