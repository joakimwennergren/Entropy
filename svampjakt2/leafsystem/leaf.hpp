#pragma once

#include <vector>
#include "sprite.hpp"
#include "scenegraph.hpp"

using namespace Symbios::Global;
using namespace Symbios::Graphics::Primitives;

class Leaf
{
public:
	Leaf(float x, float y, float yOffset, float mass, float scale)
	{
		float random_mass = 40 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(60-40)));

		this->xPosition = x;
		this->yPosition = y;
		this->yOffset = yOffset;
		this->mass = random_mass;
		this->scale = scale;

		const unsigned int random_index = rand() % sprites.size();

        sprite = std::make_shared<Sprite>(sprites[random_index]);

		sprite->Scale(scale);
        sprite->Translate(x, y);

        SceneGraph::GetInstance()->renderables.push_back(sprite);


	}

	void Update(float screenWidth, float screenHeight)
	{
		this->force += 1.0 * 9.8;
		float acceleration = this->force / this->mass; 
		this->yVelocity += acceleration * this->time;
		float xVelocity = this->yVelocity * this->xVelocity;

		float height = 1440 * 2.0;

    	sprite->Scale(scale);
    	sprite->ZIndex(2);
    	sprite->Rotate(this->yVelocity * 0.5, 3);
    	sprite->Translate(this->xPosition - xVelocity, this->yVelocity - (this->yOffset * -1.0f));

		if(this->yVelocity > screenHeight - this->yOffset)
		{
			float random_height = screenHeight + rand() % 400;
			float random_x = rand() % (int)screenWidth;
			float random_scale = (10.0f) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(40.0f-10.0f)));
			this->xPosition = random_x;
			this->yOffset = random_height * -1.0;
			this->yVelocity = 0.0;
			this->xVelocity = 0.05 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.8-0.05)));
			this->time = 0.0;
			this->mass = 40.0  + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100.0-40.0)));
			this->scale = random_scale;
			this->force = 0.0;
		}

  		this->time += 0.001;
	}

private: 
	float force = 0.0;
	float xPosition = 0.0;
	float yPosition = 0.0;
	float yOffset = 0.0;
	float xVelocity = 0.0;
	float yVelocity = 0.0;
	float time = 0.0;
	float mass = 0.0;
	float scale = 0.0;
	std::shared_ptr<Sprite> sprite;
	const std::vector<std::string> sprites = {"leaf1.png", "leaf2.png", "leaf3.png", "leaf4.png"};
};