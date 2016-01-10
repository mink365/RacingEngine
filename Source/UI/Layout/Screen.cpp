#include "Screen.h"

namespace re {

void Screen::setRealFrameSize(const Size &size)
{
    this->setRealFrameSize(size.width, size.height);
}

void Screen::setRealFrameSize(float screenWidth, float screenHeight) {
    _realSize.width = screenWidth;
    _realSize.height = screenHeight;

    this->updateScale();
}

float Screen::getFinalScale() const {
    return _finalScale;
}

void Screen::setDesignSize(float width, float height)
{
    this->_designSize.width = width;
    this->_designSize.height = height;

     this->updateScale();
}

void Screen::updateScale()
{
    _finalScale = 1;

    float scaleX = _realSize.width / _designSize.width;
    float scaleY = _realSize.height / _designSize.height;

    if (_realSize.height / _realSize.width > _designSize.height / _designSize.width) {
        _finalScale = scaleX;
    } else {
        _finalScale = scaleY;
    }

    _scaledSize.width = _realSize.width / _finalScale;
    _scaledSize.height = _realSize.height / _finalScale;

    _fitScreenScale.x = _scaledSize.width / _designSize.width;
    _fitScreenScale.y = _scaledSize.height / _designSize.height;
}

float Screen::getWidth() const {
    return _scaledSize.width;
}

float Screen::getHeight() const {
    return _scaledSize.height;
}

const Size &Screen::getSize() const
{
    return _scaledSize;
}

const Size &Screen::getRealSize() const
{
    return this->_realSize;
}

const Vec2 &Screen::getFitScreenScale() const
{
    return this->_fitScreenScale;
}

}
