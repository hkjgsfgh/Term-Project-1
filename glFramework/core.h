#pragma once
// glad的引用需要在glfw的上方
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// GLM的头文件
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/string_cast.hpp> // 查看矩阵内容

/*
在core.h中引用glad和GLFW后,其他文件只需要#include "core.h"即可,解决了头文件重复引用的问题,因为core.h只会被编译一次
*/