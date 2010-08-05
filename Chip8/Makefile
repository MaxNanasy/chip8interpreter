CC = g++
CFLAGS = -c -Wall
SDLFLAGS = `sdl-config --cflags --libs`
SOURCES = main.cpp cpu.cpp display.cpp file_parser.cpp sound.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = Chip8Int.exe
OUTPUT = stderr.txt stdout.txt
RM = /bin/rm

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(SDLFLAGS) -o $@

# .cpp.o:
#  $(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -rf *~ *.o $(EXECUTABLE) $(OUTPUT)
