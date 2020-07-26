// 
// Game.h
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
// 

#ifndef Game_h
#define Game_h

class Game {
public:
    Game( void );
    
    bool initialize( void );

    bool loadMedia( void );
    
    void gameLoop( void );
    
    void handleInput( SDL_Event &event );
    
    void update( void );
    
    void draw( void );
    
    void closeMedia( void );
    
    void closeGame( void );

private:
    SDL_Window *gWindow;
    SDL_Renderer *gRenderer;
    SDL_Color gColor;
    std::string gTitle;
    
    bool hasQuit;
};

#endif /* Game_h */
