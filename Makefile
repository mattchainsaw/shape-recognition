all: medialAxis

medialAxis: medialAxis.cpp
	g++ medialAxis.cpp -lCGAL -lgmp -o medialAxis
