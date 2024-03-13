#include "label.hpp"

using namespace Entropy::Text;

Label::Label(std::shared_ptr<ServiceLocator> serviceLocator, std::shared_ptr<Font> font)
{
    _serviceLocator = serviceLocator;
    _font = font;
}