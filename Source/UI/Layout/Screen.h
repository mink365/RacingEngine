#ifndef __Screen__
#define __Screen__

#include "Base/Singleton.h"
#include "Math/BoundingVolume.h"

namespace re {

class Screen : public re::Singleton<Screen>
{
public:
    static const float DESIGN_WIDTH;
    static const float DESIGN_HEIGHT;
    static const float MIN_ASPECT_RATIO;
    
    void init(float screenWidth, float screenHeight);
public:
    float getWidth() const;
    float getHeight() const;
    Size getSize() const;

    float getScaledDesignedWidth() const;
    float getScaledDesignedHeight() const;
    float getFitScreenScaleX() const;
    float getFitScreenScaleY() const;
    float getFinalScale() const;
private:
    float _realWidth;
    float _realHeight;
    float _scaledDesignWidth;
    float _scaledDesignHeight;
    float _fitScreenScaleX;
    float _fitScreenScaleY;
    float _finalScale;
};

}

#endif /* defined(__Screen__) */
