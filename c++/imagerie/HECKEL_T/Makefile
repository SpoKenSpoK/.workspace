GXX=g++
CXXFLAGS=-Wall -pedantic
DEL=rm -f

# Adaptez ces deux variables avec vos propres chemins s'ils sont différents :
INCPATH_JPEG=
LDPATH_JPEG=

all: essai.exe correction.exe

Image.o : Image.cpp Image.h
	$(GXX) -g $(CXXFLAGS) $(INCPATH_JPEG) -c Image.cpp

essai.exe : essai.cpp Image.h Image.o
	$(GXX) $(CXXFLAGS) $(INCPATH_JPEG) -g -o essai.exe essai.cpp Image.o $(LDPATH_JPEG) -ljpeg

correction.o : compilator.exe Image.h
	./compilator.exe $(INCPATH_JPEG)

correction.exe : correction.o Image.h Image.o
	$(GXX) $(CXXFLAGS) -o correction.exe correction.o Image.o $(LDPATH_JPEG) -ljpeg

clean:
	$(DEL) *.o essai.exe correction.exe

