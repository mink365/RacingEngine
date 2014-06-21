#include "Screen.h"

namespace re {

const float Screen::DESIGN_WIDTH = 614.0f;
const float Screen::DESIGN_HEIGHT = 1024.0f;
const float Screen::MIN_ASPECT_RATIO = 640 / 960.0;

void Screen::init(float screenWidth, float screenHeight) {
    _realWidth = screenWidth;
    _realHeight = screenHeight;
    _finalScale = 1;

    float scaleX = screenWidth / DESIGN_WIDTH;
    float scaleY = screenHeight / DESIGN_HEIGHT;

    if (screenHeight / screenWidth > DESIGN_HEIGHT / DESIGN_WIDTH) {
        _finalScale = scaleX;
    } else {
        _finalScale = scaleY;
    }

    _scaledDesignWidth = screenWidth / _finalScale;
    _scaledDesignHeight = screenHeight / _finalScale;

    _fitScreenScaleX = _scaledDesignWidth / DESIGN_WIDTH;
    _fitScreenScaleY = _scaledDesignHeight / DESIGN_HEIGHT;
}

float Screen::getFinalScale() const {
    return _finalScale;
}

float Screen::getWidth() const {
    return _realWidth;
}

float Screen::getHeight() const {
    return _realHeight;
}

Size Screen::getSize() const
{
    return Size(_realWidth, _realHeight);
}

float Screen::getScaledDesignedWidth() const {
    return _scaledDesignWidth;
}

float Screen::getScaledDesignedHeight() const {
    return _scaledDesignHeight;
}

float Screen::getFitScreenScaleX() const {
    return _fitScreenScaleX;
}

float Screen::getFitScreenScaleY() const {
    return _fitScreenScaleY;
}

}
