#Makefile pour les shaders
#TP21 - Déformation d'un plan par un VS
#R. RAFFIN, Dept Info, Arles

compilateur = g++
preproc_options = -c -Wall
link_options = -Wall -lGLEW -lGL -lGLU -lglut
main = tpdeformation

all: $(main)
	@echo "\ncompilation terminee"

$(main): fonctions.o objetsOFF.o glsl_fs.o glsl_vs.o glsl_program.o tpdeformation.o
	@echo "--- compilation de $@ ..."
	$(compilateur) $^ $(link_options) -o $@
	@echo "--- compilation de $@ terminée"

fonctions.o: fonctions.cpp fonctions.h
	$(compilateur) $(preproc_options) $<

glsl_fs.o: glsl_fs.cpp glsl_fs.h
	$(compilateur) $(preproc_options) $<

glsl_program.o: glsl_program.cpp glsl_program.h
	$(compilateur) $(preproc_options) $<

glsl_vs.o: glsl_vs.cpp glsl_vs.h
	$(compilateur) $(preproc_options) $<

objetsOFF.o: objetsOFF.cpp objetsOFF.h
	$(compilateur) $(preproc_options) $<

tpdeformation.o: tpdeformation.cpp
	$(compilateur) $(preproc_options) $<

clean:
	rm -f *.o $(main) *~

archive:
	tar czvf $(main).tgz *.cpp *.h makefileDeformation *.vert *.frag

