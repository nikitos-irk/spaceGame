# spaceGame
![Test Image 1](SpaceGamePlay.gif)

## Build
```Shell
conan remote add public-conan https://api.bintray.com/conan/bincrafters/public-conan True
mkdir build
cd build
conan install ..
cmake ..
cmake --build .
```

## Run
```Shell
./bin/spaceGame
```
