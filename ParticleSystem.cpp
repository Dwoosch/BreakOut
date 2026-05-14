#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	particles.resize(100);
}

void ParticleSystem::Emit(int count, float x, float y, float lifetime, Tmpl8::Pixel color)
{
	// emit 'count' particles at the specified position with random velocities and the given lifetime and color
	for (int i = 0; i < count; i++)
	{
		for (auto& particle : particles)
		{
			if (particle.active)
				continue;

			particle.x = x;
			particle.y = y;
			particle.dx = ((rand() % 200) - 100) / 100.0f * particle.velocity;
			particle.dy = ((rand() % 200) - 100) / 100.0f * particle.velocity;
			particle.lifetime = lifetime;
			particle.maxLifetime = lifetime;
			particle.active = true;
			particle.color = color;
			break; // emit one particle per loop iteration
		}
	}
}

void ParticleSystem::Update(float deltaTime)
{
	// update each active particle's position based on its velocity and decrease its lifetime by deltaTime
	for (auto& particle : particles)
	{
		if (!particle.active)
			continue;
		particle.x += particle.dx;
		particle.y += particle.dy;
		particle.lifetime -= deltaTime;
		if (particle.lifetime <= 0)
		{
			particle.active = false; // deactivate the particle when its lifetime is over
		}
	}
}

void ParticleSystem::Draw(Tmpl8::Surface* surface)
{
	// draw each active particle as a circle on the surface, fading it out as it ages
	for (const auto& particle : particles)
	{
		if (particle.active)
		{
			// fade out the particle as it ages
			Tmpl8::Pixel fadedColor = ScalePixelColor(particle.color, particle.lifetime / particle.maxLifetime);
			surface->Circle((int)particle.x, (int)particle.y, 2, fadedColor);
		}
	}
}

Tmpl8::Pixel ParticleSystem::ScalePixelColor(Tmpl8::Pixel c, float weight)
{
	// extract RGB components, scale them by the weight, and recombine into a single pixel value
	int r = (c >> 16) & 0xFF;
	int g = (c >> 8) & 0xFF;
	int b = c & 0xFF;
	r = (int)(r * weight);
	g = (int)(g * weight);
	b = (int)(b * weight);
	return (r << 16) | (g << 8) | b;
}