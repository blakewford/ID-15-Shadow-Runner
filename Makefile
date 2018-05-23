CC=g++

runner: main.cpp
	$(CC) -DPROGMEM= main.cpp -o $@ -I. -include port.h

clean:
	rm runner
