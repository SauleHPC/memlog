CXXFLAGS=-std=c++20 -I ./include
LDFLAGS=-lglfw -lGL
LD=g++

view:


OBJS= opengl.o glad.o
opengl: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
