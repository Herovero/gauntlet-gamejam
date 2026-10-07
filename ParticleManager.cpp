#include "ParticleManager.hpp"
#include <cmath>

Particle::Particle() : active(false) {}

void Particle::Spawn(Vector2 pos, Vector2 vel, Color tint) {
    position = pos;
    velocity = vel;
    color = tint;
    maxLife = (float)GetRandomValue(30, 60) / 100.0f; // 0.3s to 0.6s lifetime
    life = maxLife;
    active = true;
}

void Particle::Update(float dt) {
    if (!active) return;
    
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    
    // Add gravity and drag
    velocity.y += 400.0f * dt; 
    velocity.x *= 0.95f;       
    
    life -= dt;
    if (life <= 0.0f) active = false;
}

void Particle::Draw() {
    if (!active) return;
    float alpha = life / maxLife;
    Color drawColor = Fade(color, alpha);
    float rotation = life * 500.0f; 
    
    Rectangle dest = { position.x, position.y, 8.0f, 8.0f };
    DrawRectanglePro(dest, { 4.0f, 4.0f }, rotation, drawColor);
}

bool Particle::IsActive() { return active; }

ParticleManager::ParticleManager(int poolSize) {
    pool.resize(poolSize);
}

void ParticleManager::EmitCoinBurst(Vector2 position) {
    int particlesToSpawn = 5;
    for (int i = 0; i < particlesToSpawn; i++) {
        for (auto& p : pool) {
            if (!p.IsActive()) {
                float angle = (float)GetRandomValue(0, 360) * (PI / 180.0f);
                float speed = (float)GetRandomValue(150, 350);
                Vector2 vel = { std::cos(angle) * speed, std::sin(angle) * speed - 100.0f };
                p.Spawn(position, vel, GOLD);
                break; 
            }
        }
    }
}

void ParticleManager::Update(float dt) {
    for (auto& p : pool) p.Update(dt);
}

void ParticleManager::Draw() {
    for (auto& p : pool) p.Draw();
}