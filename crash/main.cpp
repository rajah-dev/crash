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

struct dynamicRect {
    SDL_Rect rect;
    SDL_FPoint velocity;
};


bool pointInRect( SDL_Point &point, SDL_Rect &rect ); //SDL_PointInRect dummy
bool rectVsRect( SDL_Rect &rectA, SDL_Rect &rectB );
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

bool rectVsRect( dynamicRect &dynamic, SDL_Rect &target, SDL_FPoint &contactPoint,
                SDL_Point &contactNormal, float &collisionTime, float &elapsedTime )
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
    
    if( rayVsRect( centerPoint( dynamic.rect ), dynamic.velocity, expandedTarget, contactPoint, contactNormal, collisionTime, elapsedTime ) )
    {
        if( collisionTime <= 1.0f ) //
        {
            return true;
        }
    }

    return false;
}

int main(int argc, const char * argv[]) {

    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    
    Game crash;
    
    if( crash.initialize( gWindow, gRenderer ) )
    {
        SDL_Event event;
        bool hasQuit = false;
        
        SDL_Point originPos = { 550, 400 };
        SDL_Point mousePos = { 0, 0 };
        
        SDL_FPoint rayDirection = { 0, 0 };
        SDL_FPoint cPoint = { 0.0, 0.0 };
        SDL_Point cNormal = { 0, 0 };
        float time = 0;
        
        SDL_Color gColor = { 0x33, 0x13, 0x5C, 255 };
        SDL_Color obColor = { 0xDE, 0x38, 0xC8, 255 };
        SDL_Color highlight = { 0xFF, 0xD3, 0x00, 255 };
        
        SDL_Rect obstacle = { 300, 200, 50, 50 };
        SDL_FRect hit = { cPoint.x, cPoint.y, 5, 5 };
        

        
        
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
            obColor = { 255, 255, 255, 255 };
            
            //A ray is P (origin)  and a D(irection vector) * T(ime)
            //Then D (rayDir) = Q (endpoint) - P (origin)
            rayDirection.x = mousePos.x - originPos.x;
            rayDirection.y = mousePos.y - originPos.y;
            hit.x = cPoint.x;
            hit.y = cPoint.y;
        
            //draw
            
            SDL_SetRenderDrawColor( gRenderer, gColor.r, gColor.g, gColor.b, gColor.a );
            SDL_RenderClear( gRenderer );
            
            SDL_SetRenderDrawColor( gRenderer, highlight.r, highlight.g, highlight.b, highlight.a );
            SDL_RenderDrawLine( gRenderer, originPos.x, originPos.y, mousePos.x, mousePos.y);
            
            if( rayVsRect(originPos, rayDirection, obstacle, cPoint, cNormal, time) && time <= 1.0f )
            {
                SDL_SetRenderDrawColor( gRenderer, obColor.r, obColor.g, obColor.b, obColor.a );
                SDL_RenderFillRectF( gRenderer, &hit );
                SDL_RenderDrawLine( gRenderer, cPoint.x, cPoint.y, cPoint.x + cNormal.x*20, cPoint.y + cNormal.y*20 );
                
                SDL_SetRenderDrawColor( gRenderer, highlight.r, highlight.g, highlight.b, highlight.a );
            }
            else
            {
                SDL_SetRenderDrawColor( gRenderer, obColor.r, obColor.g, obColor.b, obColor.a );
            }
            SDL_RenderFillRect( gRenderer, &obstacle );
            


            
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

//possibly may want all of these to be float vectors
bool rayVsRect( SDL_Point &rayOrigin, SDL_FPoint &rayDir, SDL_Rect &rect,
               SDL_FPoint &contactPoint, SDL_Point &contactNormal, float &tHitNear)
{
    //Calculate the collision point nearest to the ray origin, based on x, y axis
    //A ray is P (origin)  and a D(irection vector) * T(ime 0 - 1)
    SDL_FPoint tNear = { 0, 0 };
    tNear.x = ( rect.x - rayOrigin.x ) / rayDir.x;
    tNear.y = ( rect.y - rayOrigin.y ) / rayDir.y;
    
    //Calculate the collision point furthers from the ray origin, based on x, y axis
    SDL_FPoint tFar = { 0, 0 };
    tFar.x = ( rect.x + rect.w - rayOrigin.x ) / rayDir.x;
    tFar.y = ( rect.y + rect.h - rayOrigin.y ) / rayDir.y;
    
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

