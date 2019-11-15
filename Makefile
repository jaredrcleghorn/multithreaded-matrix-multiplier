all: dist/package dist/compute

debug: DEBUG = -DDEBUG
debug: all

dist/package: dist obj/package.o obj/structs/Matrix.o obj/structs/Message.o obj/functions/packageDotProduct.o obj/functions/processDotProduct.o
	gcc -o dist/package -pthread -Wall obj/package.o obj/structs/Matrix.o obj/structs/Message.o obj/functions/packageDotProduct.o obj/functions/processDotProduct.o

dist/compute: dist obj/compute.o obj/structs/Message.o obj/functions/computeDotProduct.o
	gcc -o dist/compute -pthread -Wall obj/compute.o obj/structs/Message.o obj/functions/computeDotProduct.o

obj/package.o: obj src/package.c src/constants.h src/structs/Matrix.h src/functions/packageDotProduct.h src/functions/processDotProduct.h
	gcc -c -o obj/package.o -std=c99 -D_GNU_SOURCE $(DEBUG) -Wall src/package.c

obj/compute.o: obj src/compute.c src/constants.h src/functions/computeDotProduct.h
	gcc -c -o obj/compute.o -std=c99 -D_GNU_SOURCE $(DEBUG) -Wall src/compute.c

obj/structs/Matrix.o: obj/structs src/structs/Matrix.c src/structs/Matrix.h
	gcc -c -o obj/structs/Matrix.o -std=c99 $(DEBUG) -Wall src/structs/Matrix.c

obj/structs/Message.o: obj/structs src/structs/Message.c src/structs/Message.h
	gcc -c -o obj/structs/Message.o -std=c99 $(DEBUG) -Wall src/structs/Message.c

obj/functions/packageDotProduct.o: obj/functions src/functions/packageDotProduct.c src/functions/packageDotProduct.h src/structs/Message.h
	gcc -c -o obj/functions/packageDotProduct.o -std=c99 -D_GNU_SOURCE $(DEBUG) -Wall src/functions/packageDotProduct.c

obj/functions/processDotProduct.o: obj/functions src/functions/processDotProduct.c src/functions/processDotProduct.h src/structs/Message.h
	gcc -c -o obj/functions/processDotProduct.o -std=c99 -D_GNU_SOURCE $(DEBUG) -Wall src/functions/processDotProduct.c

obj/functions/computeDotProduct.o: obj/functions src/functions/computeDotProduct.c src/functions/computeDotProduct.h src/structs/Message.h
	gcc -c -o obj/functions/computeDotProduct.o -std=c99 -D_GNU_SOURCE $(DEBUG) -Wall src/functions/computeDotProduct.c

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
