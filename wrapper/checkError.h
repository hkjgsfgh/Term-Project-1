#pragma once

// 预编译宏
// 如果定义了DEBUG则开启错误检查checkError
// DEBUG可以再CMakeLists文件中加入
#ifdef DEBUG
#define GL_CALL(func) func;checkError();
#else
#define GL_CALL(func) func;
#endif

void checkError();