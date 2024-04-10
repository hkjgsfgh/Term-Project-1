#include "shader.h"
#include "../wrapper/checkError.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 声明装入shader代码的两个string
	std::string vertexCode, fragmentCode;

	// 声明用于读取vs和fs文件的inFileStream
	std::ifstream vShaderFile,fShaderFile;

	// 保证ifstream遇到问题时可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// 1. 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// 2. 将文件输入流当中的字符串输入到stringStream中
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// 3. 关闭文件
		vShaderFile.close();
		fShaderFile.close();

		// 4. 将字符串从stringStream中读取出来,转化到Code String中
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch(std::ifstream::failure& e){
		std::cout << "Shader文件错误: " << e.what() << std::endl;
	}

	
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// 1. 创建Shader程序
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// 2. 为shader程序输入shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL); // 以/0结尾,不需要告诉字符串长度
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);


	int success = 0;
	char infoLog[1024];
	// 3. 执行shader代码编译
	glCompileShader(vertex);
	// 检查vertex编译结果
	checkShaderErrors(vertex, "COMPILE");

	glCompileShader(fragment);
	checkShaderErrors(fragment, "COMPILE");

	// 4. 创建一个Program壳子
	mProgram = glCreateProgram();

	// 5. 将vs和fs编译好的结果放入program壳子中
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	// 6. 执行program的链接操作,形成最终可执行的shader程序 
	glLinkProgram(mProgram);
	// 检查链接错误
	checkShaderErrors(mProgram, "LINK");

	// 清理
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
			std::cout << "Error: Shader编译错误" << "\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infoLog);
			std::cout << "Error: Program链接错误" << "\n" << infoLog << std::endl;
		}
	}
	else {
		std::cout << "输入的检查类型错误" << std::endl;
	}

	return;

}


void Shader::setFloat(const std::string& name, float value) {
	// 1.通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));

	// 2.通过Location更新Uniform变量的值
	GL_CALL(glUniform1f(location, value));

	return;
}

void Shader::setVector3(const std::string& name, float x, float y, float z) {
	GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));

	GL_CALL(glUniform3f(location, x, y, z));

	return;
}

// 重载
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
	// 第三个参数transpose: 是否对传输进去的矩阵进行转置
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	return;
}