cmake_minimum_required (VERSION 3.11)
project ("CMakeProject1")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 1. BUSCAR OPENGL EN EL SISTEMA (Esto faltaba)
find_package(OpenGL REQUIRED)

# 2. DESCARGAR GLFW
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
# Esto hace que el .exe no dependa de tener instalado Visual C++ Redistributable
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
FetchContent_MakeAvailable(glfw)

# 3. CREAR EL EJECUTABLE
# Asegúrate de que glad.c esté en la misma carpeta que este archivo .txt
add_executable (CMakeProject1 "src/CMakeProject1.cpp" "CMakeProject1.h" "glad.c"  "src/Renderer.h" "src/Renderer.cpp" "src/VertexBuffer.h" "src/VertexBuffer.cpp")


target_link_libraries(CMakeProject1 PRIVATE glfw ${OPENGL_LIBRARIES})
target_include_directories(CMakeProject1 PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(CMakeProject1 PRIVATE glfw ${OPENGL_LIBRARIES})
target_include_directories(CMakeProject1 PRIVATE .)

# Copia la carpeta de shaders al directorio donde se construye el ejecutable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

#VV para que no salga la consola VV
#if(MSVC)
#    # Esto permite usar int main() en lugar de WinMain() aunque sea una App de Windows
#    set_target_properties(CMakeProject1 PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif()