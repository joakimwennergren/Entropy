#include "assets.hpp"

using namespace Symbios::Global;

Assets *Assets::pinstance_{nullptr};
std::mutex Assets::mutex_;

Assets *Assets::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Assets();
    }
    return pinstance_;
}
