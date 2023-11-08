CC=
LD=
CFLAGS=
LDFLAGS=

all: printrandom

linear.o:
	$(CC) $(CFLAGS) -c linear.c -o linear.o

random.o:
	$(CC) $(CFLAGS) -c random.c -o random.o

main.o:
	$(CC) $(CFLAGS) -c main.c -o main.o

printrandom: linear.o random.o main.o
	$(LD) $(LDFLAGS) -rdynamic linear.o random.o main.o -o printrandom

clean:
	rm -f *.o printrandom

rebuild: clean all