#include"../include/game.hpp"
#include"../include/TextureManager.hpp"
//g++ main.cpp game.cpp -Iinclude -lSDL2 -lSDL2main -lSDL2_image
//g++ *.cpp -Iinclude -lSDL2 -lSDL2main -lSDL2_image


Game * game = nullptr;

int main(int argc, char* args[])
{

    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;


    game = new Game();
    game->init("Particle Sim", 300,200, 1080,720, false);


    while(game->running())
    {

        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();

        frameTime  = SDL_GetTicks()- frameStart;


        if(frameDelay > frameTime){
            SDL_Delay(frameDelay-frameTime);
        }

    }
    game->cleanup();


    return 0;

}