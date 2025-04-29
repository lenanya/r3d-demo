all: build/main

build/main: src/main.c include/car.h
	cc -o build/main src/main.c -Llib -Iinclude -lr3d -lm -lraylib

include/car.h: assets/car.png
	python tools/image-to-c.py assets/car.png include/car.h

clean:
	rm -rf build/*
	rm include/car.h