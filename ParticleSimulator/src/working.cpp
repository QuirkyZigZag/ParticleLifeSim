/*#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NUM_PARTICLES = 500;
const int NUM_TYPES = 5;

// Physics parameters
const float MAX_SPEED = 3.0f;
const float FRICTION = 0.98f;
const float FORCE_SCALE = 10.0f;

// Particle structure
struct Particle {
    float x, y;
    float vx, vy;
    int type;

    Particle(float x, float y, int type) : x(x), y(y), vx(0), vy(0), type(type) {}
};

// Random number generator
std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<float> randomPosX(0, SCREEN_WIDTH);
std::uniform_real_distribution<float> randomPosY(0, SCREEN_HEIGHT);
std::uniform_int_distribution<int> randomType(0, NUM_TYPES - 1);
std::uniform_real_distribution<float> randomAttraction(-5.0f, 5.0f);

// Particle system
std::vector<Particle> particles;
float attractionMatrix[NUM_TYPES][NUM_TYPES];

// Generate a symmetric attraction matrix
void generateAttractionMatrix() {
    for (int i = 0; i < NUM_TYPES; i++) {
        for (int j = i; j < NUM_TYPES; j++) {
            float value = randomAttraction(rng);
            attractionMatrix[i][j] = value;
            attractionMatrix[j][i] = value;
        }
    }
}

// Apply attraction/repulsion forces
void updateParticles() {
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = 0; j < particles.size(); j++) {
            if (i == j) continue;

            Particle& p1 = particles[i];
            Particle& p2 = particles[j];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < 1.0f) continue;

            float force = attractionMatrix[p1.type][p2.type] / (dist * dist + 1.0f);
            p1.vx += (dx / dist) * force * FORCE_SCALE;
            p1.vy += (dy / dist) * force * FORCE_SCALE;
        }
    }

    // Apply velocity, friction, and boundary constraints
    for (auto& p : particles) {
        p.vx *= FRICTION;
        p.vy *= FRICTION;

        float speed = sqrt(p.vx * p.vx + p.vy * p.vy);
        if (speed > MAX_SPEED) {
            p.vx = (p.vx / speed) * MAX_SPEED;
            p.vy = (p.vy / speed) * MAX_SPEED;
        }

        p.x += p.vx;
        p.y += p.vy;

        if (p.x < 0 || p.x > SCREEN_WIDTH) p.vx *= -1;
        if (p.y < 0 || p.y > SCREEN_HEIGHT) p.vy *= -1;
    }
}

// SDL2 setup
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = true;

// Particle colors
SDL_Color colors[NUM_TYPES] = {
    {255, 0, 0, 255},   // Red
    {0, 255, 0, 255},   // Green
    {0, 0, 255, 255},   // Blue
    {255, 255, 0, 255}, // Yellow
    {255, 0, 255, 255}  // Purple
};

// Initialize SDL2
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    window = SDL_CreateWindow("Particle Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return renderer != nullptr;
}

// Render particles
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (const auto& p : particles) {
        SDL_SetRenderDrawColor(renderer, colors[p.type].r, colors[p.type].g, colors[p.type].b, 255);
        SDL_Rect rect = {static_cast<int>(p.x), static_cast<int>(p.y), 4, 4};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

// Clean up SDL
void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Main function
int main(int argc, char* argv[]) {
    if (!initSDL()) return -1;

    generateAttractionMatrix();

    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles.emplace_back(randomPosX(rng), randomPosY(rng), randomType(rng));
    }

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        updateParticles();
        render();
        SDL_Delay(16);
    }

    cleanup();
    return 0;
}
*/