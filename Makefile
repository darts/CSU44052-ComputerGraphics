TARGET = graphics

.PHONY: clean

default: all
all: $(TARGET)

%.o: %.c
	gcc -c -Werror -o $@ $<

$(TARGET): main.o
	gcc -o $@ $^

clean:
	@rm -rf $(TARGET)
	@rm -rf *.o
