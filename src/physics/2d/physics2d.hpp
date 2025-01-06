#pragma once

#include <box2d/box2d.h>
#include <cameras/icamera_manger.hpp>
#include <flecs.h>

#include "iphysics2d.hpp"

namespace Entropy::Physics {
    /**
     * @brief Initializes the Physics2D system.
     *
     * This constructor sets up the necessary services and initializes the Box2D physics world
     * with a default world definition which includes setting the gravity.
     *
     * The ServiceLocator is used to retrieve necessary services such as the camera manager.
     *
     * Initializes the Box2D world object with a gravity vector of (0.0f, -10.0f).
     */
    struct Physics2D final : ServiceBase<IPhysics2D> {
        /**
         * @brief Initializes the Physics2D system.
         *
         * This constructor sets up the necessary services and initializes the Box2D physics world
         * with a default world definition which includes setting the gravity.
         *
         * The ServiceLocator is used to retrieve necessary services such as the camera manager.
         *
         * Initializes the Box2D world object with a gravity vector of (0.0f, -10.0f).
         */
        Physics2D() {
            const ServiceLocator *sl = ServiceLocator::GetInstance();
            _cameraManager = sl->getService<ICameraManager>();
            b2WorldDef worldDef = b2DefaultWorldDef();
            worldDef.gravity = (b2Vec2){0.0f, -10.0f};
            _world = b2CreateWorld(&worldDef);
        }

        /**
         * @brief Retrieves the identifier for the Box2D world.
         *
         * This method provides access to the internal id representing the current Box2D world.
         *
         * @return An identifier for the Box2D world.
         */
        b2WorldId Get() override { return _world; }

        /**
         * @brief Creates a kinematic body in the Box2D world.
         *
         * This method sets up a kinematic body at the specified position (x, y) with the given width (w)
         * and height (h). The body and shape definitions are configured, and the body is added to the
         * Box2D physics world.
         *
         * @param x X-coordinate for the position of the body.
         * @param y Y-coordinate for the position of the body.
         * @param w Width of the body.
         * @param h Height of the body.
         * @return The identifier of the created kinematic body.
         */
        b2BodyId CreateKinematicBody(const float x, const float y, const float w, const float h) override {
            const float ppm = _cameraManager->currentCamera->PPM;
            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.position = b2Vec2{x, y};
            bodyDef.type = b2_kinematicBody;
            const b2Polygon groundBox = b2MakeBox(w / ppm, h / ppm);
            const b2BodyId body = b2CreateBody(_world, &bodyDef);
            const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
            b2CreatePolygonShape(body, &groundShapeDef, &groundBox);
            return body;
        }

        /**
         * @brief Creates a dynamic body in the Box2D world.
         *
         * This method sets up a dynamic body at the specified position (x, y) with the given width (w)
         * and height (h). The body and shape definitions are configured, and the body is added to the
         * Box2D physics world.
         *
         * @param x X-coordinate for the position of the body.
         * @param y Y-coordinate for the position of the body.
         * @param w Width of the body.
         * @param h Height of the body.
         * @return The identifier of the created dynamic body.
         */
        b2BodyId CreateDynamicBody(const float x, const float y, const float w, const float h) override {
            const float ppm = _cameraManager->currentCamera->PPM;
            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.position = b2Vec2{x, y};
            bodyDef.type = b2_dynamicBody;
            const b2Polygon groundBox = b2MakeBox(w / ppm, h / ppm);
            const b2BodyId body = b2CreateBody(_world, &bodyDef);
            const b2ShapeDef groundShapeDef = b2DefaultShapeDef();
            b2CreatePolygonShape(body, &groundShapeDef, &groundBox);
            return body;
        }

    private:
        b2WorldId _world{};
        std::shared_ptr<ICameraManager> _cameraManager;
    };
} // namespace Entropy::Physics
