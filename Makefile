CXX = clang++
SDL = -framework SDL2
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall -c -std=c++11
LDFLAGS = $(SDL)
EXE = result

all: $(EXE)


$(EXE): main.o game.o background.o spaceship.o speed.o colorschema.o explosion.o common.o
	$(CXX) $(LDFLAGS) -o result common.o explosion.o colorschema.o speed.o spaceship.o background.o game.o main.o #$< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp #$< -c $@

spaceship.o: SpaceShip.cpp SpaceShip.hpp
	$(CXX) $(CXXFLAGS) SpaceShip.cpp #$< -c $@

game.o: Game.cpp Game.hpp
	$(CXX) $(CXXFLAGS) Game.cpp #$< -c $@

background.o: Background.cpp Background.hpp
	$(CXX) $(CXXFLAGS) Background.cpp #$< -c $@

speed.o: speed.cpp speed.hpp
	$(CXX) $(CXXFLAGS) speed.cpp

colorschema.o: colorschema.cpp colorschema.hpp
	$(CXX) $(CXXFLAGS) colorschema.cpp

explosion.o: explosion.cpp explosion.hpp
	$(CXX) $(CXXFLAGS) explosion.cpp

common.o: common.cpp common.hpp
	$(CXX) $(CXXFLAGS) common.cpp

clean:
	rm *.o && rm $(EXE)
