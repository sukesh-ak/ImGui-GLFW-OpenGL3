cmake -S . -B build/ -DOpenGL_GL_PREFERENCE=GLVND
cd build/
cmake --build .
./ImGui-GLFW-OpenGL3
