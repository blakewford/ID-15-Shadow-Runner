runner: main.cpp
	g++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

clang-runner: main.cpp
	clang++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

ppc-runner: main.cpp
	powerpc-linux-gnu-g++ -gdwarf-4 -std=c++11 -nostdlib main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

mips-runner: main.cpp
	mips-linux-gnu-g++ -gdwarf-4 -std=c++11 -nostdlib main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

x86-runner: main.cpp
	g++ -m32 -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

avr-runner: main.cpp
	avr-g++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -include port.h -include avr/pgmspace.h -Wno-narrowing -fpermissive -mmcu=atmega32u4 -no-pie

arm-runner: main.cpp
	arm-linux-gnueabi-g++ -fno-exceptions -nostdlib -gdwarf-4 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

aarch64-runner: main.cpp
	aarch64-linux-gnu-g++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

clean:
	rm avr-runner runner arm-runner aarch64-runner clang-runner ppc-runner mips-runner
