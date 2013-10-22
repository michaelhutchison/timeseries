#  List of all programs you want to build
EX=main simulator
OBJ=graphicslib.o View.o Light.o Scene.o Mouse.o Texture.o vec3.o Model.o simulator.o
DATA=timeseries.rcd
#  Libraries - Linux
LIBS=-lglut -lGLU -lGL
#  Libraries - OSX
#LIBS=-framework GLUT -framework OpenGL
#  Libraries - MinGW
#LIBS=-lglut32cu -lglu32 -lopengl32

#  Main target
all: $(EX) $(DATA)

timeseries.rcd: simulator
	./simulator $(DATA) 
    

simulator: simulator.o Model.o vec3.o
	g++ -O -Wall simulator.o Model.o vec3.o -o simulator 

#  Generic compile rules
.cpp.o:
	g++ -c -O -Wall $< -IGL

#  Generic compile and link
%: %.cpp graphicslib.a
	g++ -Wall -O3 -o $@ $^ $(LIBS)

#  Delete unwanted files
clean:
	rm -f $(EX) $(DATA) *.o *.a

#  Create archive (include glWindowPos here if you need it)
graphicslib.a: $(OBJ) 
	ar -rcs graphicslib.a $^

