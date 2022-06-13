Options := -g -Wall
Headers := src/linklist.h src/global.h src/main.h src/print.h src/tools.h src/task.h

all: 
	gcc ${Options} src/main.c -c -o build/main.o
	gcc ${Options} src/linklist.c -c -o build/linklist.o
	gcc ${Options} src/task.c -c -o build/task.o
	gcc ${Options} src/print.c -c -o build/print.o
	gcc ${Options} src/tools.c -c -o build/tools.o

	gcc ${Headers} build/*.o -o ./focus

run:
	if [ -e focus ]; then ./focus ;fi

clean:
	for i in build/*.o; do rm "$$i"; done
	if [ -e focus ]; then rm focus ;fi