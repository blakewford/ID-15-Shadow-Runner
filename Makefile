runner: main.cpp
	$(CC) -O0 -target $(Target) -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -Wno-narrowing -fpermissive -fno-pie

clang-runner: main.cpp
	clang++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -fno-pie

ppc-runner: main.cpp
	powerpc-linux-gnu-g++ -gdwarf-4 -std=c++11 -nostdlib main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

mips-runner: main.cpp
	mips-linux-gnu-g++ -gdwarf-4 -std=c++11 -nostdlib main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

x86-runner: main.cpp
	$(CC) -O0 -target x86_64-pc-linux-gnu -m32 -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -no-pie

avr-runner: main.cpp
	$(CC) -c -O0 -target avr-atmel-none -gdwarf-4 -std=c++11 main.cpp -o $@ -I/usr/lib/avr/include -I. -include port.h -include avr/pgmspace.h -Wno-narrowing -fpermissive -mmcu=atmega32u4 -no-pie

arm-runner: main.cpp
	arm-linux-gnueabi-g++ -fno-exceptions -nostdlib -gdwarf-4 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -fno-pie

aarch64-runner: main.cpp
	$(CC) -c -O0 -target aarch64-unknown-linux-gnu -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive -fno-pie

clean:
	rm avr-runner runner arm-runner aarch64-runner clang-runner ppc-runner mips-runner

CC=clang
Target=x86_64-pc-linux-gnu
