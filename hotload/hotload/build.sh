#!/bin/bash
c++ -shared -undefined dynamic_lookup -o game.so game.cpp -I/Library/Frameworks/SDL2.framework/Headers -I/. -framework SDL2 -framework Cocoa
c++ main.cpp -o main -I/Library/Frameworks/SDL2.framework/Headers -I/. -framework SDL2 -framework Cocoa