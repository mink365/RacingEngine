#ifndef BUTTON_H
#define BUTTON_H

#include "UI/Widget.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/Label.h"
#include "PreDeclare.h"

namespace re {
namespace ui {

enum ButtonType
{
    Image,
};

class ImageButtonData
{
public:
    ImageButtonData() = default;
    ImageButtonData(const string& texDefault, const string& texPress);
    ImageButtonData(const string& texDefault, const string& texPress, const string& texDis);

public:
    TextureFrame::ptr defaultSprite, pressedSprite, disabledSprite;
};

class Button : public Component<Widget>
{
    using ButtonDataType = std::tuple<ImageButtonData>;

public:
    Button();

    void switchState(WidgetState newState);

    void initTouchListener();

    void setOnClickFunc(std::function<void(ButtonPtr &)> func);

    template<ButtonType type>
    typename std::tuple_element<EnumToInt(type), ButtonDataType>::type& getData()
    {
        return std::get<EnumToInt(type)>(datas);
    }

public:
    void registerEvents();

protected:
    void onAwake();

protected:
    void copyProperties(const Button& rhs);

protected:
    void initGraphic();
    void switchStateForImage(WidgetState oldState, WidgetState newState);

protected:
    std::function<void(ButtonPtr& button)> onButtonClickFunc;

    bool isTouchDown;
    int touchDownTime;

    WidgetState state;

    ButtonDataType datas;
};

} // namespace ui
} // namespace re

#endif // BUTTON_H
