#递归将本文件夹下所有cpp放到APP中
file(GLOB_RECURSE APP ./  *.cpp)

#将APP中所有cpp编译为app这个lib库
add_library(app ${APP} )