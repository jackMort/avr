CC=gcc
CFLAGS=-Wall
OBJS=seiho_temp.o

seiho_temp: $(OBJS)
	$(CC) $(CFLAGS) -o seiho_temp $(OBJS)

seiho_temp.o:
	$(CC) $(CFLAGS) -c seiho_temp.c -o seiho_temp.o

run: seiho_temp 
	./seiho_temp

clean:
	rm seiho_temp *.o
