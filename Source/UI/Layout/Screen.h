#ifndef __Screen__
#define __Screen__

#include "Base/Singleton.h"
#include "Math/BoundingVolume.h"

namespace re {

class Screen : public re::Singleton<Screen>
{
public:
    void setDesignSize(float width, float height);
    void setRealFrameSize(const Size& size);
    void setRealFrameSize(float screenWidth, float screenHeight);
public:
    float getWidth() const;
    float getHeight() const;
    const Size& getSize() const;

    const Size& getRealSize() const;
    const Vec2& getFitScreenScale() const;
    float getFinalScale() const;
private:
    Size _designSize;
    Size _realSize;
    Size _scaledSize;
    Vec2 _fitScreenScale;
    float _finalScale;
};

}

#endif /* defined(__Screen__) */
