#pragma once
#include <future>
#include <obj/model.hpp>

namespace Entropy {
namespace Components {
struct OBJModel {
  std::shared_ptr<Entropy::OBJ::ObjModel> model;
};
} // namespace Components
} // namespace Entropy
