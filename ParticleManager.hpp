#pragma once
#include "raylib.h"
#include <vector>

class Particle {
private:
    Vector2 position;
    Vector2 velocity;
    float life;
    float maxLife;
    Color color;
    bool active;

public:
    Particle();
    void Spawn(Vector2 pos, Vector2 vel, Color tint);
    void Update(float dt);
    void Draw();
    bool IsActive();
};

class ParticleManager {
private:
    std::vector<Particle> pool;

public:
    ParticleManager(int poolSize = 50);
    void EmitCoinBurst(Vector2 position);
    void Update(float dt);
    void Draw();
};