#pragma once

// Ԥ�����
// ���������DEBUG����������checkError
// DEBUG������CMakeLists�ļ��м���
#ifdef DEBUG
#define GL_CALL(func) func;checkError();
#else
#define GL_CALL(func) func;
#endif

void checkError();