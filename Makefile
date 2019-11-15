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

obj/structs/Message.o: obj/structs src/structs/Message.c src/structs/Message.h
	gcc -c -o obj/structs/Message.o $(DEBUG) -Wall src/structs/Message.c

obj/functions/packageDotProduct.o: obj/functions src/functions/packageDotProduct.c src/functions/packageDotProduct.h src/structs/Message.h
	gcc -c -o obj/functions/packageDotProduct.o $(DEBUG) -Wall src/functions/packageDotProduct.c

dist:
	mkdir -p dist

obj:
	mkdir -p obj

obj/structs: obj
	mkdir -p obj/structs

obj/functions: obj
	mkdir -p obj/functions

clean:
	rm -fr dist obj
