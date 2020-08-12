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

namespace aabb {

    struct dynaRect {
        SDL_Rect rect;
        SDL_FPoint vel;
        SDL_FPoint vrPos;
        SDL_FPoint getCenterF()
        {
            return { rect.x + ( rect.w / 2.0f ) , rect.y + ( rect.h / 2.0f ) };
        }
        SDL_Point getCenter()
        {
            return { rect.x + ( rect.w / 2 ) , rect.y + ( rect.h / 2 ) };
        }
    };

    bool pointInRect( const SDL_Point &point, const SDL_Rect &target )
    {
        return ( point.x >= target.x && //Is point right of rect's right
                point.y >= target.y && //Is point below rect's top
                point.x < target.x + target.w && //Is point left of rect's right
                point.y < target.y + target.h ); //Is point above rect's bottom
    }

    bool rectVsRect( const SDL_Rect &rectA, const SDL_Rect &rectB )
    {
        return ( rectA.x < rectB.x + rectB.w && //Is A's left side to the left of B's right?
                rectA.x + rectA.w > rectB.x && //Is A's right side to the right of B's left?
                rectA.y < rectB.y + rectB.h && //Is A's top above B's bottom?
                rectA.y + rectA.h > rectB.y ); //Is A's bottom below B's top?
    }

    bool rayVsRect( const SDL_Point &rayOrigin, const SDL_FPoint &rayDir, const SDL_Rect &rect,
                   SDL_Point &contactPoint, SDL_Point &contactNormal, float &tHitNear)
    {
        contactPoint = { 0,0 };
        contactNormal = { 0,0 };
        
        
        SDL_FPoint invDir = { 1.0f / rayDir.x, 1.0f / rayDir.y};
        
        //Calculate the collision point nearest to the ray origin, based on x, y axis
        //A ray is P (origin)  and a D(irection vector) * T(ime 0 - 1)
        SDL_FPoint tNear;
        tNear.x = ( rect.x - rayOrigin.x ) * invDir.x;
        tNear.y = ( rect.y - rayOrigin.y ) * invDir.y;
        
        //Calculate the collision point furthers from the ray origin, based on x, y axis
        SDL_FPoint tFar;
        tFar.x = ( rect.x + rect.w - rayOrigin.x ) * invDir.x;
        tFar.y = ( rect.y + rect.h - rayOrigin.y ) * invDir.y;
        
        //if far point is closer than near point, because of origin's perspective relative to rectangle
        //we need to sort them
        if( tNear.x > tFar.x ) std::swap( tNear.x, tFar.x );
        if( tNear.y > tFar.y ) std::swap( tNear.y, tFar.y );
        
        //simple checks to see if ray intersects, if not (false)
        if( tNear.x > tFar.y || tNear.y > tFar.x ) return false;
        
        //if it will intersect, find the nearest and furthest
        //tHitNear is essentially time, if it is less than one, then there has been a collision
        tHitNear = std::max( tNear.x, tNear.y );
        float tHitFar = std::min( tFar.x, tFar.y );
        
        //not interested if collision happens behind the rays direction
        //implies closest hit point will also be negative
        if( tHitFar < 0 ) return false;
        
        //can assume normal based on near.x vs near.y
        contactPoint.x = round(rayOrigin.x + tHitNear * rayDir.x);
        contactPoint.y = round(rayOrigin.y + tHitNear * rayDir.y);
        
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

    bool dynaRectVsRect( const dynaRect &dynamic, const SDL_Rect &target, SDL_Point &contactPoint,
                    SDL_Point &contactNormal, float &collisionTime, const float elapsedTime )
    {
        //assume rects don't start in collision, so if velocity is 0, just return false
        if( dynamic.vel.x == 0 && dynamic.vel.y == 0 )
        {
            return false;
        }
        
        //expand size of target rectangle to include buffer, based on dynamic rectangle
        SDL_Rect expandedTarget;
        expandedTarget.x = target.x - round( dynamic.rect.w / 2 );
        expandedTarget.y = target.y - round( dynamic.rect.h / 2 );
        expandedTarget.w = target.w + dynamic.rect.w;
        expandedTarget.h = target.h + dynamic.rect.h;
        
        
        SDL_Point center = { dynamic.rect.x + ( dynamic.rect.w / 2 ), dynamic.rect.y + ( dynamic.rect.h / 2 ) };
        SDL_FPoint velXtime = { dynamic.vel.x * elapsedTime, dynamic.vel.y * elapsedTime };
        if( rayVsRect( center, velXtime, expandedTarget, contactPoint, contactNormal, collisionTime ) )
        {
            return (collisionTime >= 0.0f && collisionTime <= 1.0f );
        }
        else
        {
            return false;
        }
    }

} //end namespace aabb

int main(int argc, const char * argv[]) {

    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    
    Game crash;
    
    if( crash.initialize( gWindow, gRenderer ) )
    {
        SDL_Event event;
        bool hasQuit = false;
               
        SDL_Color gColor = { 0x33, 0x13, 0x5C, 255 };
        SDL_Color obColor = { 0xDE, 0x38, 0xC8, 255 };
        SDL_Color highlight = { 0xFF, 0xD3, 0x00, 255 };
        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Color black = { 0, 0, 0, 255 };
        
                       
        Uint32 startTime = SDL_GetTicks();
        float elapsedTime = 0;
        
        SDL_Point mousePos;
        SDL_Rect target = { 150, 250, 300, 75 };
        
        std::vector<SDL_Rect> obstacles;
        
        obstacles.push_back( { 50, 300, 50, 50 } );
        obstacles.push_back( { 100, 300, 50, 50 } );
        obstacles.push_back( { 150, 300, 50, 50 } );
        obstacles.push_back( { 200, 300, 50, 50 } );
        obstacles.push_back( { 250, 300, 50, 50 } );
        obstacles.push_back( { 300, 300, 50, 50 } );
        obstacles.push_back( { 400, 300, 50, 50 } );
        obstacles.push_back( { 450, 300, 50, 50 } );
        obstacles.push_back( { 550, 100, 50, 50 } );
        obstacles.push_back( { 550, 150, 50, 50 } );
        obstacles.push_back( { 550, 200, 50, 50 } );
        obstacles.push_back( { 550, 250, 50, 50 } );
        obstacles.push_back( { 550, 300, 50, 50 } );
        obstacles.push_back( { 550, 350, 50, 50 } );
        obstacles.push_back( { 550, 400, 50, 50 } );
        obstacles.push_back( { 550, 450, 50, 50 } );
        
        aabb::dynaRect player;
        player.rect = { 24, 24, 24, 24 };
        
        SDL_FPoint rayDirection = { 0, 0 };

        SDL_Point cPoint;
        SDL_Point cNormal;
        
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
                            case SDLK_w:
                                rayDirection.y -= 1;
                                break;
                            case SDLK_s:
                                rayDirection.y += 1;
                                break;
                            case SDLK_a:
                                rayDirection.x -= 1;
                                break;
                            case SDLK_d:
                                rayDirection.x += 1;
                                break;
                    }
                }
                if( event.type == SDL_KEYUP && event.key.repeat == 0 )
                {
                    //keysym member = what key was pressed/released
                    switch( event.key.keysym.sym )
                    {
                            case SDLK_w:
                                rayDirection.y += 1;
                                break;
                            case SDLK_s:
                                rayDirection.y -= 1;
                                break;
                            case SDLK_a:
                                rayDirection.x += 1;
                                break;
                            case SDLK_d:
                                rayDirection.x -= 1;
                                break;
                    }
                }
            }
            startTime = SDL_GetTicks();
            //UPDATE
            player.vel.x += rayDirection.x * 200.0f * elapsedTime;
            player.vel.y += rayDirection.y * 200.0f * elapsedTime;
            float cTime = 0;
            
            std::vector< std::pair< int, float > > obList;
            
            for( int i = 0; i < obstacles.size(); i++ )
            {
                if( aabb::dynaRectVsRect( player, obstacles[i], cPoint, cNormal, cTime, elapsedTime ) )
                {
                    obList.push_back({ i, cTime });
                }
            }
            std::sort(obList.begin(), obList.end(), []( const std::pair<int, float> &a, const std::pair<int, float> &b)
                  {
                return a.second < b.second;
                    }
                      );
            
            for( auto j : obList )
            {
                cTime = 0;
                if( aabb::dynaRectVsRect( player, obstacles[j.first], cPoint, cNormal, cTime, elapsedTime ) )
                {
                    player.vel.x += cNormal.x * abs(player.vel.x) * ( 1 - cTime );
                    player.vel.y += cNormal.y * abs(player.vel.y) * ( 1 - cTime );
                }
            }
            

            
            player.rect.x += round(player.vel.x * elapsedTime);
            player.rect.y += round(player.vel.y * elapsedTime);
            
            
            //DRAW
            SDL_SetRenderDrawColor( gRenderer, gColor.r, gColor.g, gColor.b, gColor.a);
            SDL_RenderClear( gRenderer );
            
            SDL_SetRenderDrawColor( gRenderer, white.r, white.g, white.b, white.a);
            for( int i = 0; i < obstacles.size(); i++ )
            {
                SDL_RenderDrawRect( gRenderer, &obstacles[i] );
            }
            
            SDL_SetRenderDrawColor( gRenderer, highlight.r, highlight.g, highlight.b, highlight.a);
            SDL_RenderFillRect( gRenderer, &player.rect );
            
            SDL_SetRenderDrawColor( gRenderer, white.r, white.g, white.b, white.a);
            SDL_RenderDrawLine( gRenderer, player.getCenterF().x, player.getCenterF().y, player.getCenterF().x + player.vel.x, player.getCenterF().y + player.vel.y );
            
            SDL_RenderPresent( gRenderer );
            elapsedTime = (SDL_GetTicks() - startTime) / 1000.0f;
        }
    }

    crash.closeGame( gWindow, gRenderer );
    
    return 0;
}

