#pragma once
// glad��������Ҫ��glfw���Ϸ�
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// GLM��ͷ�ļ�
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/string_cast.hpp> // �鿴��������

/*
��core.h������glad��GLFW��,�����ļ�ֻ��Ҫ#include "core.h"����,�����ͷ�ļ��ظ����õ�����,��Ϊcore.hֻ�ᱻ����һ��
*/