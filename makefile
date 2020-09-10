
CXX=gcc


all: main

main: 
	mkdir -p output
	$(CXX) -c ./original/original_algorithm.c
	$(CXX) -c ./parallel/parallel_algorithm.c
	$(CXX) -c decrypt_encrypt_functions.c
	$(CXX) -pthread  parallel_algorithm.o decrypt_encrypt_functions.o -o parallel_version
	$(CXX)  original_algorithm.o decrypt_encrypt_functions.o -o original_version

	
clean:
	rm -f *.o parallel_version original_version