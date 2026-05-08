#pragma once
#include <array>
#include "surface.h"

struct Particle
{
	float x, y;
	float dx, dy;
	const float velocity = 3;
	float lifetime; // remaining life of the particle in seconds 
	float maxLifetime; // total lifetime of the particle in seconds
	bool active; // indicates whether the particle is currently active
	Tmpl8::Pixel color;
};

class ParticleSystem
{
public:
	std::array<Particle, 100> particles; // pool of particles

	void Emit(int count, float x, float y, float lifetime, Tmpl8::Pixel color);
	void Update(float deltaTime);
	void Draw(Tmpl8::Surface* surface);
	Tmpl8::Pixel ScalePixelColor(Tmpl8::Pixel c, float weight);
};