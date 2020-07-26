// 
// main.cpp
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
// 

#include <string>

#include <SDL2/SDL.h>

#include "include/Game.h"

int main(int argc, const char * argv[]) {

    Game crash;
    
    if( crash.initialize() )
    {
        crash.gameLoop();
    }
    
    crash.closeGame();
    
    return 0;
}
