# 添加可执行文件和源文件CMAKE_CURRENT_SOURCE_DIR表示CMakeLists.txt所在的目录
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#添加头文件（根据实际的安装目录修改，linux上）
set(Json_Include ${CMAKE_CURRENT_SOURCE_DIR}/include/nlohmann)
# 创建可执行文件
add_executable(TaskSystem_exec ${SOURCES} "CommandWrapper.h")
# 添加 nlohmann 头文件路径
target_include_directories(TaskSystem_exec
    PRIVATE 
    ${Json_Include}
    # 添加其他目录路径...
)
# 输出包含路径以供调试
message(STATUS "Json Include Dir: ${Json_Include}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TaskSystem_exec PROPERTY CXX_STANDARD 20)
endif()

