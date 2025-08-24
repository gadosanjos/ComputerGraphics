g++ glew_glfw_test.cpp -o glew_glfw_test -lglfw -lGLEW -lGL
g++ red_teapot_test.cpp -o red_teapot_test -lglut -lGLU -lGL -lm
nm -D /usr/lib/x86_64-linux-gnu/libglut.so | grep Teapot
g++ soil_quad_test.cpp -o soil_quad_test -lSOIL -lglut -lGLU -lGL -lm
