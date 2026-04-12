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
add_executable (CMakeProject1 WIN32 "CMakeProject1.cpp" "CMakeProject1.h" "glad.c")

# 4. VINCULAR TODO
# Usamos ${OPENGL_LIBRARIES} que es la variable que rellena find_package
target_link_libraries(CMakeProject1 PRIVATE glfw ${OPENGL_LIBRARIES})

# Esto ayuda a que encuentre los headers de OpenGL y GLFW
target_include_directories(CMakeProject1 PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(CMakeProject1 PRIVATE glfw ${OPENGL_LIBRARIES})

# ESTA LÍNEA ES LA QUE SOLUCIONA EL ERROR DE 'glad/glad.h'
# Le dice a CMake que busque headers en la carpeta donde está este CMakeLists.txt
target_include_directories(CMakeProject1 PRIVATE .)
if(MSVC)
    # Esto permite usar int main() en lugar de WinMain() aunque sea una App de Windows
    set_target_properties(CMakeProject1 PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()