all :
	gcc -Wall -Wextra -g rtl.c -o realtimeloading.exe

init:
	./realtimeloading.exe init

config:
	./realtimeloading.exe config

clean:
	rm *.exe *.txt
