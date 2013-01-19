
CFLAGS = -I ../include
OBJS = test_main.o

all: $(OBJS)
	$(CC) -o test $(OBJS) ../bin/libcintelhex.so -lcunit

clean:
	rm -f $(OBJS) test
