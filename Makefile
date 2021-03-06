CXX=g++
CXXFLAGS=-g -Wall
LDFLAGS=-g
SDLCFLAGS=`pkg-config --cflags sdl2`
SDLLIBS=`pkg-config --libs sdl2`

ifeq (NOT_SDL,1)
GLEWCFLAGS=`pkg-config --cflags glew`
GLEWLIBS=`pkg-config --libs glew`
OPENGLCFLAGS=`pkg-config --cflags opengl`
OPENGLLIBS=`pkg-config --libs opengl`
else
GLEWCFLAGS=-DSDL_GLEW=1
GLEWLIBS=
OPENGLCFLAGS=-DSDL_GLSYMBOL=1
OPENGLLIBS=
endif


all: sdl2_opengl

clean:
	rm -f *.o dice-data-generator sdl2_opengl \
	cubic-vert.cpp cubic-geom.cpp cubic-frag.cpp \
	dice-vert.cpp dice-geom.cpp dice-frag.cpp \
	floor-vert.cpp floor-frag.cpp \
	lightstar-vert.cpp lightstar-geom.cpp lightstar-frag.cpp

dist-clean: clean
	rm -f dice-data-generator dice-data.cpp cubic-data-generator cubic-data.cpp

.INTERMEDIATE: dice-data-generator cubic-data-generator

.PHONY: all clean dist-clean

sdl2_opengl: \
	sdl2_opengl.o \
	cubic.o \
	dice.o \
	floor.o \
	lightstar.o
	$(CXX) $^ $(LDFLAGS) $(SDLLIBS) $(GLEWLIBS) $(OPENGLLIBS) -o $@

sdl2_opengl.o: \
	sdl2_opengl.cpp \
	sdl2_opengl.hh \
	cubic.hh \
	dice.hh \
	floor.hh \
	lightstar.hh
	$(CXX) $(CXXFLAGS) $(SDLCFLAGS) $(GLEWCFLAGS) $(OPENGLCFLAGS) $< -o $@ -c

cubic.o: \
	cubic.cpp \
	sdl2_opengl.hh \
	cubic.hh \
	cubic-data.cpp \
	cubic-vert.cpp \
	cubic-geom.cpp \
	cubic-frag.cpp
	$(CXX) $(CXXFLAGS) $(SDLCFLAGS) $(GLEWCFLAGS) $(OPENGLCFLAGS) $< -o $@ -c

dice.o: \
	dice.cpp \
	dice.hh \
	dice-data.cpp \
	dice-vert.cpp \
	dice-geom.cpp \
	dice-frag.cpp
	$(CXX) $(CXXFLAGS) $(SDLCFLAGS) $(GLEWCFLAGS) $(OPENGLCFLAGS) $< -o $@ -c

floor.o: \
	floor.cpp \
	sdl2_opengl.hh \
	floor-vert.cpp \
	floor-frag.cpp \
	border-vert.cpp \
	border-frag.cpp \
	floor.hh
	$(CXX) $(CXXFLAGS) $(SDLCFLAGS) $(GLEWCFLAGS) $(OPENGLCFLAGS) $< -o $@ -c

lightstar.o: \
	lightstar.cpp \
	sdl2_opengl.hh \
	lightstar.hh \
	lightstar-vert.cpp \
	lightstar-geom.cpp \
	lightstar-frag.cpp
	$(CXX) $(CXXFLAGS) $(SDLCFLAGS) $(GLEWCFLAGS) $(OPENGLCFLAGS) $< -o $@ -c

cubic-data.cpp: \
	cubic-data-generator
	./cubic-data-generator > $@

cubic-data-generator: \
	cubic-data-generator.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

dice-data.cpp: \
	dice-data-generator
	./dice-data-generator > $@

dice-data-generator: \
	dice-data-generator.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

border-vert.cpp: \
	border.vert
	echo "static const char *border_vertexSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

border-frag.cpp: \
	border.frag
	echo "static const char *border_fragmentSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

cubic-vert.cpp: \
	cubic.vert
	echo "static const char *cubic_vertexSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

cubic-geom.cpp: \
	cubic.geom
	echo "static const char *cubic_geometrySource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

cubic-frag.cpp: \
	cubic.frag
	echo "static const char *cubic_fragmentSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

dice-vert.cpp: \
	dice.vert
	echo "static const char *dice_vertexSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

dice-geom.cpp: \
	dice.geom
	echo "static const char *dice_geometrySource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

dice-frag.cpp: \
	dice.frag
	echo "static const char *dice_fragmentSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

floor-vert.cpp: \
	floor.vert
	echo "static const char *floor_vertexSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

floor-frag.cpp: \
	floor.frag
	echo "static const char *floor_fragmentSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

lightstar-vert.cpp: \
	lightstar.vert
	echo "static const char *lightstar_vertexSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

lightstar-geom.cpp: \
	lightstar.geom
	echo "static const char *lightstar_geometrySource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@

lightstar-frag.cpp: \
	lightstar.frag
	echo "static const char *lightstar_fragmentSource = R\"glsl(" > $@
	cat $< >> $@
	echo ")glsl\";" >> $@
