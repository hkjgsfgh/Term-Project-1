#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名字
project(01_OpenGL)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#在项目中加入一个全局的预编译宏 
add_definitions(-DDEBUG)


#打包资源文件夹assets
file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})


include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

# 链接静态库
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)


# 添加文件夹
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glFramework)


#本工程所有cpp文件编译链接，生成exe
add_executable(glStudy "glad.c""main_3D.cpp")

target_link_libraries(glStudy glfw3.lib assimp-vc143-mt.lib wrapper app fw)
