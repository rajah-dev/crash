// 
// main.cpp
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
//
// thanks to javidx9 (better citation needed)

#include <string>

#include <SDL2/SDL.h>

#include "include/Game.h"

struct vector {
    float x;
    float y;
};

bool pointInRect( SDL_Point &point, SDL_Rect &rect ); //SDL_PointInRect dummy
bool rectVsRect( SDL_Rect &rectA, SDL_Rect &rectB );

bool rayVsRect( SDL_Point &rayOrigin, SDL_Point &rayDir, SDL_Rect &rect,
               SDL_Point contactPoint, SDL_Point contactNormal, int tHitNear)
{
    SDL_Point tNear = { 0, 0 };
    tNear.x = ( rect.x - rayOrigin.x ) / rayDir.x;
    tNear.y = ( rect.y - rayOrigin.y ) / rayDir.y;
    
    SDL_Point tFar = { 0, 0 };
    tFar.x = ( rect.x + rect.w - rayOrigin.x ) / rayDir.x;
    tFar.y = ( rect.y + rect.h - rayOrigin.y ) / rayDir.y;
    
    //if far point is closer than near point, because of origin's perspective relative to rectangle
    //we need to sort them
    if( tNear.x > tFar.x ) std::swap( tNear.x, tFar.x );
    if( tNear.y > tFar.y ) std::swap( tNear.y, tFar.y );
    
    //simple checks to see if ray intersects if not (false)
    if( tNear.x > tFar.y || tNear.y > tFar.x ) return false;
    
    //if it will intersect, find the nearest and furthest
    tHitNear = std::max( tNear.x, tNear.y );
    int tHitFar = std::min( tFar.x, tFar.y );
    
    //not interested if collision happens behind the rays direction
    //implies closest hit point will also be negative
    if( tHitFar < 0 ) return false;
    
    //can assume normal based on near.x vs near.y
    
    contactPoint.x = rayOrigin.x + tHitNear * rayDir.x;
    contactPoint.y = rayOrigin.y + tHitNear * rayDir.y;
    
    if( tNear.x > tNear.y )
    {
        if( rayDir.x < 0 )
            contactNormal = { 1, 0 };
        else
            contactNormal = { -1, 0};
    }
    else if ( tNear.x < tNear.y )
    {
        if( rayDir.y < 0 )
            contactNormal = { 0, 1 };
        else
            contactNormal = { 0, -1};
    }
    
    return true;
}

int main(int argc, const char * argv[]) {

    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    
    Game crash;
    
    if( crash.initialize( gWindow, gRenderer ) )
    {
        SDL_Event event;
        bool hasQuit = false;
        SDL_Point originPos = { 10, 10 };
        SDL_Point mousePos = { 0, 0 };
        SDL_Point rayDirection = { 0, 0 };
        SDL_Point cPoint = { 0, 0 };
        SDL_Point cNormal = { 0, 0 };
        int t = 0;
        SDL_Color gColor = { 113, 28, 145, 255 };
        SDL_Color oColor = { 0, 0, 0, 255 };
        SDL_Color pColor = { 255, 255, 255, 255 };
        SDL_Rect obstacle = { 300, 200, 50, 50 };
        SDL_Rect player = { 250, 150, 50, 50 };
        int velocity = 1;
        while( !hasQuit )
        {
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                    hasQuit = true;
                }
                if( event.type == SDL_MOUSEMOTION )
                {
                    mousePos = { event.motion.x, event.motion.y };
                }
                if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
                {
                    //keysym member = what key was pressed/released
                    switch( event.key.keysym.sym )
                    {

                    }
                }
            }
            oColor = { 255, 255, 255, 255 };
            //update
            rayDirection.x = mousePos.x - originPos.x;
            rayDirection.y = mousePos.y - originPos.y;
            
            if( rectVsRect( player, obstacle ) )
            {
                oColor = { 0, 0, 0, 255 };
            }
            
            //draw
            
            SDL_SetRenderDrawColor( gRenderer, gColor.r, gColor.g, gColor.b, gColor.a );
            SDL_RenderClear( gRenderer );
            
            SDL_SetRenderDrawColor( gRenderer, oColor.r, oColor.g, oColor.b, oColor.a );
            SDL_RenderFillRect( gRenderer, &obstacle );
            
            if( rayVsRect(originPos, rayDirection, obstacle, cPoint, cNormal, t)) SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
            else SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 255 );
            SDL_RenderDrawLine( gRenderer, originPos.x, originPos.y, mousePos.x, mousePos.y);

            
            //SDL_RenderDrawLine( gRenderer, originPos.x, originPos.y, mousePos.x, mousePos.y);
            
            SDL_RenderPresent( gRenderer );
        }
    }

    crash.closeGame( gWindow, gRenderer );
    
    return 0;
}

bool pointInRect( SDL_Point &point, SDL_Rect &rect )
{
    return ( point.x >= rect.x && //Is point right of rect's right
            point.y >= rect.y && //Is point below rect's top
            point.x < rect.x + rect.w && //Is point left of rect's right
            point.y < rect.y + rect.h ); //Is point above rect's bottom
}
    
bool rectVsRect( SDL_Rect &rectA, SDL_Rect &rectB )
{
    return ( rectA.x < rectB.x + rectB.w && //Is A's left side to the left of B's right?
            rectA.x + rectA.w > rectB.x && //Is A's right side to the right of B's left?
            rectA.y < rectB.y + rectB.h && //Is A's top above B's bottom?
            rectA.y + rectA.h > rectB.y ); //Is A's bottom below B's top?
}



