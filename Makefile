run: build/autocenter
	 build/autocenter assets/alpha-dolphins-001.png

build/autocenter: autocenter.c
	 gcc -ggdb `pkg-config --cflags opencv` -o build/autocenter autocenter.c `pkg-config --libs opencv`

clean:
	 rm -Rf build
	 mkdir build
