all: medialAxis

medialAxis: medialAxis.cpp MedialGraph.h
	g++ medialAxis.cpp -lCGAL -lgmp -o medialAxis


clean: 
	rm -f medialAxis
