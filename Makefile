CXX = clang++
SDL = -framework SDL2
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall -c -std=c++11
LDFLAGS = $(SDL)
EXE = result

all: $(EXE)


$(EXE): main.o spaceship.o
	$(CXX) $(LDFLAGS) -o result spaceship.o main.o #$< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp #$< -c $@

spaceship.o: SpaceShip.cpp
	$(CXX) $(CXXFLAGS) SpaceShip.cpp #$< -c $@

clean:
	rm *.o && rm $(EXE)
