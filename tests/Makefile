
CFLAGS = -I ../include
OBJS = test_main.o

all: $(OBJS)
	$(CC) -o test $(OBJS) ../obj/*.o -lcunit

clean:
	rm -f $(OBJS) test
