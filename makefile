LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -c candycrush.cpp
	g++ candycrush.o -o candycrush $(LIBS)

clean:
	rm -f candycrush