CC=gcc
LD=gcc
CFLAGS=-g -c
LDFLAGS=-rdynamic

all: printrandom

linear.o:
	$(CC) $(CFLAGS) linear.c -o linear.o

random.o:
	$(CC) $(CFLAGS) random.c -o random.o

main.o:
	$(CC) $(CFLAGS) -rdynamic main.c -o main.o

printrandom: linear.o random.o main.o
	$(LD) $(LDFLAGS) linear.o random.o main.o -o printrandom

clean:
	rm -f *.o printrandom

rebuild: clean all