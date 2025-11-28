CXXFLAGS=-std=c++20 -I ./include
LDFLAGS=-lglfw -lGL
LD=g++

view:


OBJS= opengl.o glad.o
opengl: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@


OBJS_FR= opengl_framework.o glad.o
opengl_framework: $(OBJS_FR)
	$(LD) $(OBJS_FR) $(LDFLAGS) -o $@

clean:
	-rm $(OBJS)
	-rm $(OBJS_FR)
