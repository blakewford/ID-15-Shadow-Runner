runner: main.cpp
	g++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive

avr-runner: main.cpp
	avr-g++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -include port.h -include avr/pgmspace.h -Wno-narrowing -fpermissive -mmcu=atmega32u4

arm-runner: main.cpp
	arm-none-eabi-g++ -fno-exceptions -nostdlib -gdwarf-4 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive

aarch64-runner: main.cpp
	aarch64-linux-gnu-g++ -gdwarf-4 -std=c++11 main.cpp -o $@ -I. -DPROGMEM= -include port.h -Wno-narrowing -fpermissive

clean:
	rm avr-runner runner arm-runner aarch64-runner
