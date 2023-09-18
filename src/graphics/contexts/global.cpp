#include "global.hpp"

using namespace Symbios::Graphics::Contexts;

/**
 * Static methods should be defined outside the class.
 */

Global *Global::pinstance_{nullptr};
std::mutex Global::mutex_;

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value. RU:
 */
Global *Global::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Global();
    }
    return pinstance_;
}