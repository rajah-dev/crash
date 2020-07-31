// 
// Box.cpp
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
// 

//SDL Specific
#include <SDL2/SDL.h>

//Self
#include "Box.h"

//Custom


Box::Box( int x, int y, int w, int h )
{
    mBox = { x, y, w, h };
    bColor = { 255, 255, 255, 255 };
    
    //Movement properties
    isMoveable = true;
    velocity = 0;
    vectorX = 0.0;
    vectorY = 0.0;
}

void Box::setSizeAndPosition( int x, int y, int w, int h )
{
    mBox = { x, y, w, h };
}

SDL_Rect *Box::getSizeAndPosition( void )
{
    return &mBox;
}

void Box::setColor( SDL_Color color )
{
    bColor = color;
}

void Box::setMove( bool move )
{
    isMoveable = move;
}

void Box::setVelocity( int v )
{
    velocity = v;
}

void Box::handleInput( SDL_Event &event )
{
    //SDL_KEYDOWN or SDL_KEYUP (event types) occurs when a key is pressed or released
    //repeat = non-zero if this is a key repeat
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
    {
        //keysym member = what key was pressed/released
        switch( event.key.keysym.sym )
        {
            case SDLK_w: vectorY -= velocity; break;
            case SDLK_s: vectorY += velocity; break;
            case SDLK_a: vectorX -= velocity; break;
            case SDLK_d: vectorX += velocity; break;
        }
    }
    else if( event.type == SDL_KEYUP && event.key.repeat == 0 )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_w: vectorY += velocity; break;
            case SDLK_s: vectorY -= velocity; break;
            case SDLK_a: vectorX += velocity; break;
            case SDLK_d: vectorX -= velocity; break;
        }
    }
}

void Box::move( void )
{
    if( isMoveable )
    {
        mBox.y += vectorY;
        mBox.x += vectorX;
    }
}

void Box::render( SDL_Renderer *&renderer )
{
    SDL_SetRenderDrawColor( renderer, bColor.r, bColor.g, bColor.b, bColor.a );
    SDL_RenderFillRect( renderer, &mBox );
}

