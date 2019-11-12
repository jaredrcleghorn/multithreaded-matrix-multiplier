all: dist/package dist/compute

debug: DEBUG = -DDEBUG
debug: all

dist/package: dist obj/package.o
	gcc -o dist/package -Wall obj/package.o

dist/compute: dist obj/compute.o
	gcc -o dist/compute -Wall obj/compute.o

obj/package.o: obj src/package.c
	gcc -c -o obj/package.o $(DEBUG) -Wall src/package.c

obj/compute.o: obj src/compute.c
	gcc -c -o obj/compute.o $(DEBUG) -Wall src/compute.c

obj/structs/Matrix.o: obj/structs src/structs/Matrix.c src/structs/Matrix.h
	gcc -c -o obj/structs/Matrix.o $(DEBUG) -Wall src/structs/Matrix.c

dist:
	mkdir -p dist

obj:
	mkdir -p obj

obj/structs: obj
	mkdir -p obj/structs

clean:
	rm -fr dist obj
