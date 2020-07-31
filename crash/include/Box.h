// 
// Box.h
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
// 

#ifndef Box_h
#define Box_h

class Box {
public:
    Box( int x = 0, int y = 0, int w = 0, int h = 0 );
    
    void setSizeAndPosition( int x, int y, int w, int h );
    
    SDL_Rect *getSizeAndPosition( void );
    
    void setColor( SDL_Color color );
    
    void setMove( bool move );
    
    void setVelocity( int v );
    
    void handleInput( SDL_Event &event );
    
    void move( void );
    
    void render( SDL_Renderer *&renderer );
    
private:
    //Box properties
    SDL_Rect mBox;
    SDL_Color bColor; //default { 255, 255, 255, 255 }
    
    //Movement properties
    bool isMoveable; //default false
    int velocity;  //default 0
    float vectorX, vectorY;
};

#endif /* Box_h */
