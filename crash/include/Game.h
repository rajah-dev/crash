// 
// Game.h
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
// 

#ifndef Game_h
#define Game_h

#include "Box.h"

class Game {
public:
    Game( void );
    
    bool initialize( SDL_Window *&gWindow, SDL_Renderer *&gRenderer );
    
    void closeGame( SDL_Window *&gWindow, SDL_Renderer *&gRenderer );
    
private:
    SDL_Color gColor;
    std::string gTitle;
};

#endif /* Game_h */
