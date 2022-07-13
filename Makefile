

CC = g++ 
CFLAGS = -c -Wall 
LDFLAGS = 
INCLUDE = include 

SRC = main sprite_chopper
SOURCES = $(addsuffix .cpp, $(addprefix src/, $(SRC))) 
OBJECTS = $(SOURCES:.cpp=.o) 
EXECUTABLE = sprite_chopper

main: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS) 	
	$(CC) $(OBJECTS) -o $@ -DSFML_STATIC \
		-Iinclude -Llib -lsfml-graphics -lsfml-system -lsfml-window -lsfml-network -lstdc++ 

%.o: %.cpp 	
	g++ -c -o $@ $< -I$(INCLUDE) 

clear: 	
	rm -f src/*.o $(EXECUTABLE) 


