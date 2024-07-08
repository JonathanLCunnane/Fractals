CC 		?= gcc
CFLAGS 	?= -Wall -Wextra -pedantic -std=c11 -g
TARGET = fractals
SRCS = fractals.c state.c
LIBS = -lm

# SDL2 configuration
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LIBS = $(shell sdl2-config --libs)

# Object files
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(SDL2_LIBS) $(LIBS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
