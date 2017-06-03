all: qmake-make build

qmake-make:
	qmake kpf-qt.pro -o src/Makefile

build:
	make -C src

copy:
	cp src/KPF bin/linux/KPF

clean:
	make -C src clean
	rm -rf src/Makefile
	rm -rf src/KPF
	rm -rf bin/linux/KPF
	rm -rf src/.qmake.stash
