cmake_minimum_required(VERSION 3.10)

project(notes_app)
set(CMAKE_CXX_STANDARD 17)

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add GLFW (which is in src/glfw)
add_subdirectory(src/glfw)
include_directories(src/glfw/include)




# Add ImGui
include_directories(src/imgui src/imgui/backends)
file(GLOB IMGUI_SOURCES
        src/imgui/*.cpp
        src/imgui/backends/imgui_impl_glfw.cpp
        src/imgui/backends/imgui_impl_opengl3.cpp
)

# Your app source files
add_executable(notes_app
        src/main.cpp
        ${IMGUI_SOURCES}
)

# Link with glfw and OpenGL
target_link_libraries(notes_app
        glfw
        ${OPENGL_gl_LIBRARY}
        pthread
)

add_custom_command(TARGET notes_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Users/IcePa/Downloads/i686-15.1.0-release-win32-dwarf-msvcrt-rt_v12-rev0/mingw32/bin/libgcc_s_dw2-1.dll"
        "C:/Users/IcePa/Downloads/i686-15.1.0-release-win32-dwarf-msvcrt-rt_v12-rev0/mingw32/bin/libstdc++-6.dll"
        $<TARGET_FILE_DIR:notes_app>
)
