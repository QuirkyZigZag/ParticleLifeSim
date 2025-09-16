/*#include"../include/game.hpp"
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
#include "../include/Quadtree.hpp"
#include"../include/Vector2.hpp"
#include "../include/Entity.hpp" 
#include "../include/UIComponent.h"
const float SEARCH_SCALE = 50.0f;
const float MAX_SPEED = 10.0f;


Quadtree * q;

std::vector<std::vector<double>> generateRandomMatrix(int x){
    std::vector<std::vector<double>> result(x, std::vector<double>(x));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);

    
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x; j++) {
            result[i][j] = dis(gen);
        }
    }
    return result;
}


int x = 5;
std::vector<std::vector<double>> matrix = {
{1.0,0.9,0.5,0.4,0.3},
{0.9,1.0,0.5,0.4,0.3},
{0.5,0.5,0.1,-0.2,-0.3},
{0.4,0.4,-0.2,0.1,-0.1},
{0.3,0.3,-0.3,-0.1,0.1}};




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

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool full_screen)
{
    int flags = 0;

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

    

    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,4);
    std::uniform_int_distribution<std::mt19937::result_type> screenDistX(0,1200); //0,1200
    std::uniform_int_distribution<std::mt19937::result_type> screenDistY(0,700); //0,700


    printMatrix(matrix);

    for ( int i =0;i<1000;i++){
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

    Rectangle * r = new Rectangle(0,0, 2000,1500);

    q = new Quadtree(*r);
    q->update(Particles);

    
}

void Game::handleEvents()
{
    
    SDL_PollEvent(&event);
    switch ( event.type){

        case SDL_QUIT:
            isRunning = false; break;


        default: break;
    }
}
void Game::update() {
    // Update Quadtree to handle particles 
    q->update(Particles);

    // Particle loop
    for (int i = 0; i < Particles.size(); i++) {
        auto& partic = Particles[i];
        Vector2 pos = partic->getComponent<TransformComponent>().Position;
        Vector2& velocity = partic->getComponent<PhysicsComponent>().velocity;

       
        if (pos.x <= 0 || pos.x >= 1078) { 
            velocity.x *= -0.7f; 
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

            float minDist = 3.0f; // Adjust as needed
            float safeDist = std::max(dist, minDist);
        
            float k = 1.0f; // Smooth factor
            Vector2 force = ndir * (attraction_val / ((safeDist + k) * (safeDist + k)));

            // Apply the force to the particles
            partic->getComponent<PhysicsComponent>().velocity += force;

            // Optionally apply reciprocal force to the other particle (for symmetry)
            e->getComponent<PhysicsComponent>().velocity -= force;


            if (Collision::AABB(partic->getComponent<BoxCollider>(), e->getComponent<BoxCollider>())) {
        Collision::ResolveCollision(partic->getComponent<BoxCollider>(), e->getComponent<BoxCollider>());
    }
        }
        velocity *= 0.995f;  

        
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
*/