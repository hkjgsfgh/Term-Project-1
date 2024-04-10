#include "shader.h"
#include "../wrapper/checkError.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// ����װ��shader���������string
	std::string vertexCode, fragmentCode;

	// �������ڶ�ȡvs��fs�ļ���inFileStream
	std::ifstream vShaderFile,fShaderFile;

	// ��֤ifstream��������ʱ�����׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// 1. ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// 2. ���ļ����������е��ַ������뵽stringStream��
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// 3. �ر��ļ�
		vShaderFile.close();
		fShaderFile.close();

		// 4. ���ַ�����stringStream�ж�ȡ����,ת����Code String��
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch(std::ifstream::failure& e){
		std::cout << "Shader�ļ�����: " << e.what() << std::endl;
	}

	
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// 1. ����Shader����
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// 2. Ϊshader��������shader����
	glShaderSource(vertex, 1, &vertexShaderSource, NULL); // ��/0��β,����Ҫ�����ַ�������
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);


	int success = 0;
	char infoLog[1024];
	// 3. ִ��shader�������
	glCompileShader(vertex);
	// ���vertex������
	checkShaderErrors(vertex, "COMPILE");

	glCompileShader(fragment);
	checkShaderErrors(fragment, "COMPILE");

	// 4. ����һ��Program����
	mProgram = glCreateProgram();

	// 5. ��vs��fs����õĽ������program������
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	// 6. ִ��program�����Ӳ���,�γ����տ�ִ�е�shader���� 
	glLinkProgram(mProgram);
	// ������Ӵ���
	checkShaderErrors(mProgram, "LINK");

	// ����
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return;

}

Shader::~Shader() {

}

void Shader::begin() {
	GL_CALL(glUseProgram(mProgram));
	return;
}

void Shader::end() {
	GL_CALL(glUseProgram(0));
	return;
}

void Shader::checkShaderErrors(GLuint target, std::string type) {
	int success;
	char infoLog[1024];

	if (type == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error: Shader�������" << "\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error: Program���Ӵ���" << "\n" << infoLog << std::endl;
		}
	}
	else {
		std::cout << "����ļ�����ʹ���" << std::endl;
	}

	return;

}


void Shader::setFloat(const std::string& name, float value) {
	// 1.ͨ�������õ�Uniform������λ��Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));

	// 2.ͨ��Location����Uniform������ֵ
	GL_CALL(glUniform1f(location, value));

	return;
}

void Shader::setVector3(const std::string& name, float x, float y, float z) {
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));

	GL_CALL(glUniform3f(location, x, y, z));

	return;
}

// ����
void Shader::setVector3(const std::string& name, int size, const float* values) {
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	GL_CALL(glUniform3fv(location, size, values));
	return;
}

void Shader::setInt(const std::string& name, int value) {
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	GL_CALL(glUniform1i(location, value));
	return;
}

void Shader::setMatrix4x4(const std::string& name, glm::mat4 value) {
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
	// ����������transpose: �Ƿ�Դ����ȥ�ľ������ת��
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	return;
}