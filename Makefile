CC=g++

runner: main.cpp
	$(CC) -gdwarf-4 -std=c++11 -DPROGMEM= main.cpp -o $@ -I. -include port.h -Wno-narrowing -fpermissive

clean:
	rm runner
