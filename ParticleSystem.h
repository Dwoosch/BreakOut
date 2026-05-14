#pragma once
#include <array>
#include <vector>

#include "surface.h"

class ParticleSystem
{
public:
	ParticleSystem();
	void Emit(int count, float x, float y, float lifetime, Tmpl8::Pixel color);
	void Update(float deltaTime);
	void Draw(Tmpl8::Surface* surface);
	Tmpl8::Pixel ScalePixelColor(Tmpl8::Pixel c, float weight);

private:
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

	std::vector<Particle> particles; // pool of particles

};