#include "line.hpp"

using namespace Entropy::Graphics::Primitives;

Line::Line(glm::vec3 start, glm::vec3 end, glm::vec4 color) {

  float thickness = 10.0;

  // Calculate the direction of the line
  glm::vec3 lineDir = glm::normalize(end - start);

  // Calculate a perpendicular vector for the quad thickness (using Z-axis as
  // normal)
  glm::vec2 perpendicular = glm::vec2(-lineDir.y, lineDir.x);

  // Define the half thickness
  float halfThickness = thickness / 2.0f;

  // Calculate the offset for the quad vertices
  glm::vec2 offset = perpendicular * halfThickness;

  vertices = {{{start.x + offset.x, start.y + offset.y, start.z},
               {1.0f, 1.0f, 1.0f},
               {1.0f, 0.0f}},
              {{start.x - offset.x, start.y - offset.y, start.z},
               {1.0f, 1.0f, 1.0f},
               {0.0f, 0.0f}},
              {{end.x + offset.x, end.y + offset.y, end.z},
               {1.0f, 1.0f, 1.0f},
               {0.0f, 1.0f}},
              {{end.x - offset.x, end.y - offset.y, end.z},
               {1.0f, 1.0f, 1.0f},
               {1.0f, 1.0f}}};

  vertexBuffer = std::make_unique<VertexBuffer>(vertices);
  indexBuffer = std::make_shared<IndexBuffer<uint16_t>>(indices);
  texture =
      std::make_unique<Texture>(Filesystem::GetTexturesDir() + "blank.png");
}

Line::~Line() {}
