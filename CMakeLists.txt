cmake_minimum_required(VERSION 3.28.0) # 指定所需cmake的最低版本
project(data_struct) # 项目名称
set(CMAKE_C_STANDARD 11) # 指定最低c标准

include_directories(${PROJECT_SOURCE_DIR}/Inc)
# 项目头文件目录 {PROJECT_SOURCE_DIR} **CMakeLists.txt文件所在**

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# EXECUTABLE_OUTPUT_PATH **生成的可执行文件的目录**  

aux_source_directory(${PROJECT_SOURCE_DIR}/Src SRC_LIST)
# 将指定目录下的所有源文件都打包到变量 SRC_LIST 中

add_executable(
 main  # 可执行文件名称
 ${SRC_LIST} # 包含所有需编译的源文件的变量 
)