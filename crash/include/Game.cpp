// 
// Game.cpp
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
// 

#include <stdio.h>
#include <string>

//SDL Specific
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

//Custom
#include "Game.h"
#include "global.h"



Game::Game( void )
{
    gWindow = NULL;
    gRenderer = NULL;
    gColor = { 113, 28, 145, 255 };
    gTitle = WINDOW_TITLE;
    hasQuit = false;
}

//Initialize SDL and related subsystems
//FU: Separate out subsystems
//FU: Change to SDL_Log instead of printf in the future
bool Game::initialize( void )
{
    bool success = true;
    
    //SDL_Init returns 0 on success
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        printf( "SDL could not initialize. SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow( gTitle.c_str(),
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT,
                                  SDL_WINDOW_RESIZABLE );
        
        if( !gWindow )
        {
            printf( "SDL could not create window. SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //ACC = use graphics hardware | VSYNC = no screen tearing, synchronized to refresh rate
            //LEARN: -1 is the value for the index parameter
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

            if( !gRenderer )
            {
                printf( "SDL could not associate renderer to gWindow. SDL_Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color at the very start
                SDL_SetRenderDrawColor( gRenderer, gColor.r, gColor.g, gColor.b, gColor.a );
                
                
                //Initialize the following subsystems.
                
                //flag needed for PNG loading
                int imgFlags = IMG_INIT_PNG; // IMG_INIT_PNG = 2
                //IMG_Init returns the flags that loaded successfully. Looking for 2 & 2 = 2 = True
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                
                //initialize SDL_ttf, returns: 0 on success, -1 on any error
                //LEARN MORE: https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                
                //initialize SDL_mixer
                //LEARN MORE SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}

//FU: Separate out subsystems
void Game::closeGame( void )
{
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    //Quit SDL & subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::gameLoop( void )
{
    SDL_Event event;

    while( !hasQuit )
    {
        handleInput( event );
        update();
        draw();
    }
}

void Game::handleInput( SDL_Event &event )
{
    while( SDL_PollEvent( &event ) )
    {
        if( event.type == SDL_QUIT )
        {
            hasQuit = true;
        }
    }
}

void Game::update( void )
{

}
        
void Game::draw( void )
{
    SDL_SetRenderDrawColor( gRenderer, gColor.r, gColor.g, gColor.b, gColor.a );
    SDL_RenderClear( gRenderer );
    

    SDL_RenderPresent( gRenderer );
}

