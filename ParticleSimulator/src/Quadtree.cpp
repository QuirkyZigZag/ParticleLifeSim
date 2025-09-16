#include"../include/Quadtree.hpp"

#include<iostream>


Quadtree::Quadtree(Rectangle& boundary) : boundary(boundary){}


Quadtree::~Quadtree(){

    clear();
}



void Quadtree::clear() {
    particles.clear();
    delete northeast; northeast = nullptr;
    delete northwest; northwest = nullptr;
    delete southeast; southeast = nullptr;
    delete southwest; southwest = nullptr;
    isDivided = false;
}





bool Quadtree::insert(Entity& ent){

    if(!ent.hasComponent<ParticleComponent>()){
        std::cout << "not a particle" << std::endl;
    }

    if(!boundary.ContainsParticle(ent)){
        return false;
    }

    if(particles.size() < CAPACITY){
        particles.push_back(&ent);
        return true;
    }
    if(!isDivided) subdivide();


    return (northeast->insert(ent) || northwest->insert(ent) ||
            southeast->insert(ent) || southwest->insert(ent));
}

void Quadtree::subdivide() {
    float x = boundary.x;
    float y = boundary.y;
    float w = boundary.width / 2;
    float h = boundary.height / 2;

    Rectangle ne(x + 0.5f * w, y - 0.5f * h, w, h);
    Rectangle nw(x - 0.5f * w, y - 0.5f * h, w, h);
    Rectangle se(x + 0.5f * w, y + 0.5f * h, w, h);
    Rectangle sw(x - 0.5f * w, y + 0.5f * h, w, h);

    northeast = new Quadtree(ne);
    northwest = new Quadtree(nw);
    southeast = new Quadtree(se);
    southwest = new Quadtree(sw);

    // Redistribute particles into the subdivided nodes
    for (Entity* particle : particles) {
        if (northeast->insert(*particle)) continue;
        if (northwest->insert(*particle)) continue;
        if (southeast->insert(*particle)) continue;
        if (southwest->insert(*particle)) continue;
    }
    particles.clear();
    isDivided = true;
}


std::vector<Entity*> Quadtree::query(Rectangle& range) {
    std::vector<Entity*> foundParticles;

    // If the range does not intersect with this boundary, return an empty vector
    if (!boundary.intersects(range)) return foundParticles;

    // Check particles in the current node
    for (const auto& particle : particles) {
        if (range.ContainsParticle(*particle)) {
            foundParticles.push_back(particle);
        }
    }

    // If subdivided, query all children and merge results
    if (isDivided) {
        auto neParticles = northeast->query(range);
        auto nwParticles = northwest->query(range);
        auto seParticles = southeast->query(range);
        auto swParticles = southwest->query(range);

        // Append results from children
        foundParticles.insert(foundParticles.end(), neParticles.begin(), neParticles.end());
        foundParticles.insert(foundParticles.end(), nwParticles.begin(), nwParticles.end());
        foundParticles.insert(foundParticles.end(), seParticles.begin(), seParticles.end());
        foundParticles.insert(foundParticles.end(), swParticles.begin(), swParticles.end());
    }

    return foundParticles;
}

void Quadtree::update(const std::vector<Entity*>& allParticles) {
    clear();
    //std::cout << "updated" <<std::endl;

    for (Entity* particle : allParticles) {
        insert(*particle);
    }
}






