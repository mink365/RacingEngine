#ifndef RE_UI_IMAGE_H
#define RE_UI_IMAGE_H

#include "Texture/Frame/TextureFrame.h"
#include "Graphic.h"
#include "Math/Color.h"
#include "UI/Layout/LayoutUtil.h"

namespace re {
namespace ui {

enum class ImageType
{
    Sample,
    NinePatch,
    Filled,
};

class NinePatchData
{
public:
    NinePatchData() {};
    NinePatchData(float padding)
    {
        leftPadding = padding;
        topPadding = padding;
        rightPadding = padding;
        bottomPadding = padding;
    }
    NinePatchData(float left, float top, float right, float bottom)
    {
        leftPadding = left;
        topPadding = top;
        rightPadding = right;
        bottomPadding = bottom;
    }

public:
    float leftPadding;
    float topPadding;
    float rightPadding;
    float bottomPadding;
};

class NineGrid
{
public:
    Rect lb;
    Rect rt;

    const Rect getRect(AlignType type) const;
};

class Image : public Graphic
{
    using ImageDataType = std::tuple<bool, NinePatchData, bool>;

public:
    Image()
        : Graphic()
    {};

    void setType(ImageType type)
    {
        this->type = type;
    }

    template<ImageType type>
    typename std::tuple_element<EnumToInt(type), ImageDataType>::type& getData()
    {
        return std::get<EnumToInt(type)>(datas);
    }

public:
    void setFrame(const std::string& tex);
    void setFrame(TextureFrame::ptr frame);

protected:
    void onAwake();
    void registerEvents();

protected:
    void addQuad(AlignType type, const NineGrid& vertexGrid, const NineGrid& textureGrid, const re::Color &color);
    void rebind();

    virtual void updateViewColor();

protected:
    void copyProperties(const Image &rhs);

private:
    Rect rect;
    TextureFrame::ptr frame;

    ImageType type;

    ImageDataType datas;
};

} // namespace ui
} // namespace re

#endif // RE_UI_IMAGE_H
