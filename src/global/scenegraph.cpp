#include "scenegraph.hpp"

using namespace Symbios::Global;

SceneGraph *SceneGraph::pinstance_{nullptr};
std::mutex SceneGraph::mutex_;

SceneGraph *SceneGraph::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SceneGraph();
    }
    return pinstance_;
}

void SceneGraph::Add(std::unique_ptr<Renderable> renderable)
{
    // @todo check num descriptorsets before pushing?

}