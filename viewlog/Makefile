CXXFLAGS=-std=c++20 -I ./include
LDFLAGS=-lglfw -lGL
LD=g++

view:


OBJS= opengl.o glad.o
opengl: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@


OBJS= opengl_framework.o glad.o
opengl_framework: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
