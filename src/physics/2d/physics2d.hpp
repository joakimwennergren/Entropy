#pragma once

#include <vulkan/vulkan.hpp>
// #include <box2d/box2d.h>

#include <iostream>

#include "spdlog/spdlog.h"
#include <glm/glm.hpp>

namespace Entropy
{
    namespace Physics
    {
        class Physics2D
        {
        public:
            Physics2D();

            // inline b2Body *CreateBody(float x, float y)
            //{
            /*
            b2BodyDef groundBodyDef;
            groundBodyDef.position.Set(x, y);
            b2Body *groundBody = world->CreateBody(&groundBodyDef);
            b2PolygonShape groundBox;
            groundBox.SetAsBox(50.0f, 10.0f);
            groundBody->CreateFixture(&groundBox, 0.0f);
            return groundBody;
            */
            //}

            // inline b2Body *CreateDynamicBody()
            // {
            /*
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0.0f, 4.0f);

            b2Body *body = world->CreateBody(&bodyDef);
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(1.0f, 1.0f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            body->CreateFixture(&fixtureDef);

            return body;
            */
            //}

            // b2World *world;

        private:
        };

        class DynamicBody
        {
        public:
            DynamicBody(std::shared_ptr<Physics2D> physics){
                // body = physics->CreateDynamicBody();
            };

            float GetPosition()
            {
                // auto pos = body->GetPosition();
                // return pos.y;
                return 0.1;
            }

            // b2Body *body;
        };
    }
}
