CPP="clang++"
CPP="g++"
$CPP -w -std=c++23 \
  ./Libraries/include/imgui/imgui.cpp \
  ./Libraries/include/imgui/imgui_demo.cpp \
  ./Libraries/include/imgui/imgui_draw.cpp \
  ./Libraries/include/imgui/imgui_impl_glfw.cpp \
  ./Libraries/include/imgui/imgui_impl_opengl3.cpp \
  ./Libraries/include/imgui/imgui_tables.cpp \
  ./Libraries/include/imgui/imgui_widgets.cpp \
  ./source/glad.c \
  ./source/*.cpp \
  -o path_tracer \
  -I./Libraries/include/ -I./Libraries/include/imgui -lglfw -DLIBCXX_ENABLE_INCOMPLETE_FEATURES
    
