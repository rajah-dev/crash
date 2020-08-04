// 
// main.cpp
// was created for crash in 2020
// by Vinoo Selvarajah
// gotta start somewhere ¯\_(ツ)_/¯ 
//
// thanks to javidx9 (better citation needed)

#include <string>
#include <vector>
#include <cmath>


#include <SDL2/SDL.h>

#include "include/Game.h"




bool pointInRect( SDL_Point &point, SDL_Rect &rect ); //SDL_PointInRect dummy
bool rectVsRect( SDL_Rect &rectA, SDL_Rect &rectB );
//calc rayOrigin as float, because it will be a center point
//rayDir (D) is usually endpoint - orgin
bool rayVsRect( SDL_Point &rayOrigin, SDL_FPoint &rayDir, SDL_Rect &rect,
               SDL_FPoint &contactPoint, SDL_Point &contactNormal, float &tHitNear);


SDL_Point centerPoint( SDL_Rect rect )
{
    SDL_Point center;
    center.x = rect.x + ( rect.w / 2 );
    center.y = rect.y + ( rect.h / 2 );
    return center;
}

SDL_FPoint centerFPoint( SDL_Rect rect )
{
    SDL_FPoint center;
    center.x = rect.x + ( rect.w / 2 );
    center.y = rect.y + ( rect.h / 2 );
    return center;
}

struct dynamicRect {
    SDL_Rect rect;
    SDL_FPoint velocity;
    SDL_Point getCenter()
    {
        return centerPoint( rect );
    }
};

bool dynamicRectVsRect( dynamicRect &dynamic, SDL_Rect &target, SDL_FPoint &contactPoint,
                SDL_Point &contactNormal, float &collisionTime, float elapsedTime )
{
    //assume rects don't start in collision, so if velocity is 0, just return false
    if( dynamic.velocity.x == 0 && dynamic.velocity.y == 0 )
    {
        return false;
    }
    
    //expand size of target rectangle to include buffer, based on dynamic rectangle
    SDL_Rect expandedTarget;
    expandedTarget.x = target.x - ( dynamic.rect.w / 2 );
    expandedTarget.y = target.y - ( dynamic.rect.h / 2 );
    expandedTarget.w = target.w + dynamic.rect.w;
    expandedTarget.h = target.h + dynamic.rect.h;
    
    
    
    SDL_Point center = dynamic.getCenter();
    SDL_FPoint velXtime = { dynamic.velocity.x * elapsedTime, dynamic.velocity.y * elapsedTime };
    if( rayVsRect( center, velXtime, expandedTarget, contactPoint, contactNormal, collisionTime ) )
    {
        return (collisionTime >= 0.0f && collisionTime <= 1.0f );
    }
    else
    {
        return false;
    }
}

int main(int argc, const char * argv[]) {

    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    
    Game crash;
    
    if( crash.initialize( gWindow, gRenderer ) )
    {
        SDL_Event event;
        bool hasQuit = false;
        
        SDL_FPoint rayDirection = { 0, 0 };
        SDL_FPoint cPoint;
        SDL_Point cNormal;
        float time;
        
        SDL_Color gColor = { 0x33, 0x13, 0x5C, 255 };
        SDL_Color obColor = { 0xDE, 0x38, 0xC8, 255 };
        SDL_Color highlight = { 0xFF, 0xD3, 0x00, 255 };
        
        
        dynamicRect player;
        player.rect = { 23, 23, 23, 23 };
        
        std::vector<SDL_Rect> gameRects;
        
        gameRects.push_back( { 10, 10, 10, 10 } );
        gameRects.push_back( { 150, 150, 200, 100 } );
        
        Uint32 startTime = SDL_GetTicks();
        float elapsedTime = 0;
        
        while( !hasQuit )
        {
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                    hasQuit = true;
                }
                if( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
                {
                    //keysym member = what key was pressed/released
                    switch( event.key.keysym.sym )
                    {
                            case SDLK_w:
                                rayDirection.y -= 10;
                                break;
                            case SDLK_s:
                                rayDirection.y += 10;
                                break;
                            case SDLK_a:
                                rayDirection.x -= 10;
                                break;
                            case SDLK_d:
                                rayDirection.x += 10;
                                break;
                    }
                }
                if( event.type == SDL_KEYUP && event.key.repeat == 0 )
                {
                    //keysym member = what key was pressed/released
                    switch( event.key.keysym.sym )
                    {
                            case SDLK_w:
                                rayDirection.y += 10;
                                break;
                            case SDLK_s:
                                rayDirection.y -= 10;
                                break;
                            case SDLK_a:
                                rayDirection.x += 10;
                                break;
                            case SDLK_d:
                                rayDirection.x -= 10;
                                break;
                    }
                }
            }
            startTime = SDL_GetTicks();
            
            player.velocity.x += rayDirection.x;
            player.velocity.y += rayDirection.y;
            
            player.rect.x += round(player.velocity.x * elapsedTime);
            player.rect.y += round(player.velocity.y * elapsedTime);

            if( dynamicRectVsRect( player, gameRects[1], cPoint, cNormal, time, elapsedTime ) )
            {
                player.velocity = { 0 , 0 };
            }
            
            //A ray is P (origin)  and a D(irection vector) * T(ime)
            //Then D (rayDir) = Q (endpoint) - P (origin)

        
            //draw
            
            SDL_SetRenderDrawColor( gRenderer, gColor.r, gColor.g, gColor.b, gColor.a );
            SDL_RenderClear( gRenderer );

            SDL_SetRenderDrawColor( gRenderer, highlight.r, obColor.g, obColor.b, obColor.a );
            SDL_RenderFillRect( gRenderer, &gameRects[1] );
            
            SDL_SetRenderDrawColor( gRenderer, highlight.r, highlight.g, highlight.b, highlight.a );
            SDL_RenderFillRect( gRenderer, &player.rect );
            

            
            SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
            SDL_RenderDrawLine( gRenderer, player.getCenter().x, player.getCenter().y, player.getCenter().x + player.velocity.x, player.getCenter().y + player.velocity.y );
            
            
            SDL_RenderPresent( gRenderer );
            elapsedTime = (SDL_GetTicks() - startTime) / 1000.0f;
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

//possibly may want all of these to be float vectors
bool rayVsRect( SDL_Point &rayOrigin, SDL_FPoint &rayDir, SDL_Rect &rect,
               SDL_FPoint &contactPoint, SDL_Point &contactNormal, float &tHitNear)
{
    contactPoint = { 0,0 };
    contactNormal = { 0,0 };
    
    SDL_FPoint invDir = { 1.0f / rayDir.x, 1.0f/ rayDir.y };
    
    //Calculate the collision point nearest to the ray origin, based on x, y axis
    //A ray is P (origin)  and a D(irection vector) * T(ime 0 - 1)
    SDL_FPoint tNear = { 0, 0 };
    tNear.x = ( rect.x - rayOrigin.x ) * invDir.x;
    tNear.y = ( rect.y - rayOrigin.y ) * invDir.y;
    
    //Calculate the collision point furthers from the ray origin, based on x, y axis
    SDL_FPoint tFar = { 0, 0 };
    tFar.x = ( rect.x + rect.w - rayOrigin.x ) * invDir.x;
    tFar.y = ( rect.y + rect.h - rayOrigin.y ) * invDir.y;
    
    //if far point is closer than near point, because of origin's perspective relative to rectangle
    //we need to sort them
    if( tNear.x > tFar.x ) std::swap( tNear.x, tFar.x );
    if( tNear.y > tFar.y ) std::swap( tNear.y, tFar.y );
    
    //simple checks to see if ray intersects, if not (false)
    if( tNear.x > tFar.y || tNear.y > tFar.x ) return false;
    
    //if it will intersect, find the nearest and furthest
    tHitNear = std::max( tNear.x, tNear.y );
    float tHitFar = std::min( tFar.x, tFar.y );
    
    //not interested if collision happens behind the rays direction
    //implies closest hit point will also be negative
    if( tHitFar < 0 ) return false;
    
    //can assume normal based on near.x vs near.y
    contactPoint.x = rayOrigin.x + tHitNear * rayDir.x;
    contactPoint.y = rayOrigin.y + tHitNear * rayDir.y;
    
    if( tNear.x > tNear.y )
    {
        if( invDir.x < 0 )
            contactNormal = { 1, 0 };
        else
            contactNormal = { -1, 0};
    }
    else if ( tNear.x < tNear.y )
    {
        if( invDir.y < 0 )
            contactNormal = { 0, 1 };
        else
            contactNormal = { 0, -1};
    }
    
    return true;
}

