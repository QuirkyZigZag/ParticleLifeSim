#include"../include/game.hpp"
#include"../include/TextureManager.hpp"
//#include"../include/GameObject.hpp"
#include"../include/Collision.hpp"
#include "../include/Components.hpp"
using namespace std;
#include<string>
#include<unordered_map>
#include <random>
#include <iomanip>
#include<vector>
#include<cmath>
#include "../include/Quadtree.hpp"
#include"../include/Vector2.hpp"
#include "../include/Entity.hpp" 
#include "../include/UIComponent.h"
const float SEARCH_SCALE = 100.0f;
const float MAX_SPEED = 10.0f;
float deltaTime = 0.01f;
float FrictionHalfLife = 0.02f;
float FrictionFactor = pow(0.5f, deltaTime/FrictionHalfLife);
Uint32 lastTime;

int numberParticles = 500;
int forceModifier = 10;


Quadtree * q;

std::vector<std::vector<double>> generateRandomMatrix(int x) {
    std::vector<std::vector<double>> result(x, std::vector<double>(x));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x; j++){
            double value = dis(gen);
            result[i][j] = value;
        }
    }

    return result;
}


int x = 5;
std::vector<std::vector<double>> matrix = generateRandomMatrix(5);




float randomNum(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.01, 0.01);
    return dis(gen);
}




void printMatrix(const std::vector<std::vector<double>>& matrix)
{
    for (const auto& row : matrix)
    {
        for (const auto& value : row) {
            std::cout << std::fixed << std::setprecision(2) << value << " ";
        }
        std::cout << "\n";
    }
}

SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;
SDL_Event event;


Manager manager;

std::vector<Entity*>  Particles;
//auto& p4 = manager.addEntity();
//auto & player(manager.addEntity());


const string colors[5]= {"red", "blue", "orange", "green", "purple"};
//                         0       1        2        3        4
std::unordered_map<std::string, int> colormap = {{"red",0},
                                                 {"blue", 1},
                                                 {"orange", 2},
                                                 {"green", 3},
                                                 {"purple", 4}};


Game::Game(){}
Game::~Game(){}


void Game::spawnParticles(int numParticles){
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,4);
    std::uniform_int_distribution<std::mt19937::result_type> screenDistX(0,1200); //0,1200
    std::uniform_int_distribution<std::mt19937::result_type> screenDistY(0,700); //0,700

    

    for ( int i =0;i<numParticles;i++){
        auto & p = manager.addEntity();
        auto colorcode = dist(rng);
        std::string color = colors[colorcode];
        //std::cout<<color << std::endl;
        std::string file = "../res/" + color + ".png";
        const char * filePath = file.c_str();

        float xscreenpos = static_cast<float>(screenDistX(rng));
        float yscreenpos = static_cast<float>(screenDistY(rng));

        p.addComponent<TransformComponent>(xscreenpos, yscreenpos);
        p.addComponent<ParticleComponent>(0.2f);
        p.getComponent<ParticleComponent>().color = color;
        p.addComponent<PhysicsComponent>(Vector2(0,0), Vector2(0,0));
        p.addComponent<SpriteComponent>(filePath);
        p.addComponent<BoxCollider>("p"); 
        Particles.push_back(&p);
    }
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool full_screen)
{
    
    int flags = 0;
    lastTime = SDL_GetTicks();

    if(full_screen==true)
        flags = SDL_WINDOW_FULLSCREEN;
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){

         cout << "SDL initialised succesfully" <<  endl;

        window = SDL_CreateWindow(title, xpos,ypos,width,height, flags);

        if (window)
            cout << "window was created" << endl;
        else
            cout << "window was not created" << SDL_GetError() <<endl;

        renderer = SDL_CreateRenderer(window,-1,0);

        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            cout << "renderer was created" << endl;
        }           
        else
            cout << "renderer was not created" << SDL_GetError() <<endl;

        isRunning= true;
    }
    else
    {  
        isRunning=false;
        cout << "SDL initialisation failed, error: " << SDL_GetError() << endl;
        
    }
    //instancing code

    Game::spawnParticles(500);
    Rectangle * r = new Rectangle(0,0, 2200,1700);

    q = new Quadtree(*r); // my quadtree object
    q->update(Particles);
}

float forceFunction(float distance, float attraction) {
    distance /=150;
    const float beta = 0.3f;

    if (distance < beta) {
        // Repulsive force when distance is less than beta
        return ((distance / beta) - 1);
    } 
    else if (distance >= beta) {
        // Attractive force based on attraction value when distance is between beta and 1
        return attraction * (1 - abs(2 * distance - 1 - beta)) / (1 - beta);
    } 
    else {
        // No force if distance is beyond range
        return 0;
    }
}
void Game::handleEvents()
{
    
    SDL_PollEvent(&event);
    switch ( event.type){

        case SDL_QUIT:
            isRunning = false; break;

        case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_a) {
            std::cout << "A key pressed!" << std::endl;
        }
        else if (event.key.keysym.sym == SDLK_m) {
            std::cout << "Matrix regeneration" << std::endl;
            matrix = generateRandomMatrix(5);
            printMatrix(matrix);
        }
        else if(event.key.keysym.sym == SDLK_h){
            std::cout << "#########################\n\n\n";
            std::cout << "help menu :) :\n\n";
            std::cout << "h - for help menu ( what you pressed already)\n\n";
            std::cout << "r - restart the simulation\n\n";
            std::cout << "p - respawn in particles\n\n";
            std::cout << "m - regenerate matrix\n\n";
            std::cout << "f - reset all velocities to 0\n\n";
            std::cout << "up/down keys - increase/ increase velocity of particles\n\n";
            std::cout << "shift up/down - increase force modifier of the particles\n\n";
            std::cout << "#########################\n\n\n";
        }
        else if (event.key.keysym.sym == SDLK_r) {
            std::cout << "simulation restarted" << std::endl;
            matrix = generateRandomMatrix(5);
            manager.clearEntities();
            Particles.clear();
            Game::spawnParticles(numberParticles);
            
        }
        else if (event.key.keysym.sym == SDLK_p) {
            std::cout << "particles respawning" << std::endl;
            manager.clearEntities();
            Particles.clear();
            Game::spawnParticles(numberParticles);
            
        }

        else if (event.key.keysym.sym == SDLK_f) {
            for (Entity *e :Particles){
                std::cout << " velocities reset" << std::endl;
                e->getComponent<PhysicsComponent>().SetVelocity(Vector2(0,0));
            }
            
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            std::cout << "population increased" << std::endl;
            numberParticles +=50;
            manager.clearEntities();
            Particles.clear();
            Game::spawnParticles(numberParticles);

        }
        else if (event.key.keysym.sym == SDLK_DOWN) {
            std::cout << "population decreased" << std::endl;
            numberParticles -= 50;
            manager.clearEntities();
            Particles.clear();
            Game::spawnParticles(numberParticles);
        }

        else if (event.key.keysym.sym == SDLK_UP && (SDL_GetModState() & KMOD_LSHIFT)){
            std::cout << "force modifier increased" << std::endl;
            forceModifier += 0.5;
            std::cout << "forceModifier: " << forceModifier << std::endl;
        }

        else if (event.key.keysym.sym == SDLK_DOWN && (SDL_GetModState() & KMOD_LSHIFT)){
            std::cout << "force modifier decreased" << std::endl;
            forceModifier -= 0.5;
            std::cout << "forceModifier: " << forceModifier << std::endl;
        }




        default: break;
    }
}
void Game::update() {
    
    //Uint32 currentTime =  SDL_GetTicks();
   // float deltaTime = (currentTime-lastTime)/1000;
    //lastTime = currentTime;
    // Update Quadtree to handle particles 
    q->update(Particles);

    // Particle loop
    for (int i = 0; i < Particles.size(); i++) {
        auto& partic = Particles[i];
        Vector2 pos = partic->getComponent<TransformComponent>().Position;
        Vector2& velocity = partic->getComponent<PhysicsComponent>().velocity;

       
        if (pos.x <= 0 || pos.x >= 1078) { 
            velocity.x *= -1.0f; 
        }
        if (pos.y <= 0 || pos.y >= 718) { 
            velocity.y *= -1.0f;  
        }

        
        Rectangle search_area(pos.x - SEARCH_SCALE / 2, pos.y - SEARCH_SCALE / 2, SEARCH_SCALE, SEARCH_SCALE);

        // Query nearby particles
        std::vector<Entity*> near = q->query(search_area);

        for (Entity* e : near) {
            // Skip self
            if (e == partic) continue;

            // Get the direction vector
            Vector2 dir = Vector2::getDirection(pos, e->getComponent<TransformComponent>().Position);
            float dist = Vector2::Magnitude(dir);

            // Avoid division by zero
            if (dist < 1e-6 || dist > SEARCH_SCALE) continue;

            // Normalize direction vector
            Vector2 ndir = Vector2::getNormalised(dir);

            // Get the colors of the interacting particles
            std::string p1color = partic->getComponent<ParticleComponent>().color;
            std::string p2color = e->getComponent<ParticleComponent>().color;

            // Get the indices of the colors in the colormap and attraction matrix
            int p1attractionindex = colormap.at(p1color);
            int p2attractionindex = colormap.at(p2color);
            double attraction_val = matrix[p1attractionindex][p2attractionindex];
        
            float k = 1.0f; // Smooth factor
            Vector2 force = ndir * forceFunction(dist, attraction_val);
            //Vector2 force = ndir * attraction_val;
            force *= 0.01*forceModifier;

            // Apply the force to the particles
            partic->getComponent<PhysicsComponent>().ApplyForce(force,deltaTime);

            //apply reciprocal force to the other particle (for symmetry)
            e->getComponent<PhysicsComponent>().ApplyForce(force* (-1),deltaTime);


            if (Collision::AABB(partic->getComponent<BoxCollider>(), e->getComponent<BoxCollider>())) {
        Collision::ResolveCollision(partic->getComponent<BoxCollider>(), e->getComponent<BoxCollider>());
    }
        }
        velocity *= FrictionFactor;  
    }
    
    


    // Refresh and update the manager (handle components and entities)
    manager.refresh();
    manager.update();
    count++;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Before drawing, check all entities
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::cleanup()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "memory cleaned" << endl;
}