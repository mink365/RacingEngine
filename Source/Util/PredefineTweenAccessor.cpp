#include "PredefineTweenAccessor.h"

#include "UI/Base/Transform2D.h"
#include "UI/Base/HierarchyColor.h"
#include "Tween.h"

using namespace TweenEngine;

namespace re {

using namespace ui;

void PredefineTweenAccessor::registerAccessor()
{
    auto func = [=](std::shared_ptr<void> obj, int tweenType, TweenCMD cmd, float *values)->int
    {
        Node* node = (Node*)(obj.get());
        Transform2DPtr transform = node->getComponent<Transform2D>();
        HierarchyColorPtr hierColor = node->getComponent<HierarchyColor>();

        if (cmd == TweenCMD::SET) {
            switch(tweenType) {
            case FlatNodeAccessor::POSITION_X:
            {
                const Vec2& position = transform->getPosition();
                transform->setPosition(Vec2(values[0], position.y));

                break;
            }
            case FlatNodeAccessor::POSITION_Y:
            {
                const Vec2& position = transform->getPosition();
                transform->setPosition(Vec2(position.x, values[0]));

                break;
            }
            case FlatNodeAccessor::POSITION_XY:
            {
                transform->setPosition(Vec2(values[0], values[1]));

                break;
            }
            case FlatNodeAccessor::SCALE:
            {
                transform->setScale(values[0]);

                break;
            }
            case FlatNodeAccessor::SCALE_X:
            {
                const Vec2& scale = transform->getScale();
                transform->setScale(Vec2(values[0], scale.y));

                break;
            }
            case FlatNodeAccessor::SCALE_Y:
            {
                const Vec2& scale = transform->getScale();
                transform->setScale(Vec2(scale.x, values[0]));

                break;
            }
            case FlatNodeAccessor::SCALE_XY:
            {
                transform->setScale(Vec2(values[0], values[1]));

                break;
            }
            case FlatNodeAccessor::ANCHOR_X:
            {
                const Vec2& anchor = transform->getAnchorPoint();

                transform->setAnchorPoint(Vec2(values[0], anchor.y));

                break;
            }
            case FlatNodeAccessor::ANCHOR_Y:
            {
                const Vec2& anchor = transform->getAnchorPoint();

                transform->setAnchorPoint(Vec2(anchor.x, values[0]));

                break;
            }
            case FlatNodeAccessor::ANCHOR_XY:
            {
                transform->setAnchorPoint(Vec2(values[0], values[1]));

                break;
            }
            case FlatNodeAccessor::ROTATION:
            {
                transform->setRotation(values[0]);

                break;
            }
            case FlatNodeAccessor::COLOR:
            {
                hierColor->setColor(Color(values[0], values[1], values[2], values[3]));

                break;
            }
            case FlatNodeAccessor::ALPHA:
            {
                hierColor->setAlpha(values[0]);

                break;
            }
            case FlatNodeAccessor::OPACITY:
            {
                hierColor->setAlpha(values[0] / 255);

                break;
            }
            }
        } else if (cmd == TweenCMD::GET) {
            switch(tweenType) {
            case FlatNodeAccessor::POSITION_X:
            {
                const Vec2& position = transform->getPosition();

                values[0] = position.x;

                break;
            }
            case FlatNodeAccessor::POSITION_Y:
            {
                const Vec2& position = transform->getPosition();

                values[0] = position.y;

                break;
            }
            case FlatNodeAccessor::POSITION_XY:
            {
                const Vec2& position = transform->getPosition();

                values[0] = position.x;
                values[1] = position.y;

                break;
            }
            case FlatNodeAccessor::SCALE:
            {
                const Vec2& scale = transform->getScale();

                values[0] = scale.x;

                break;
            }
            case FlatNodeAccessor::SCALE_X:
            {
                const Vec2& scale = transform->getScale();

                values[0] = scale.x;

                break;
            }
            case FlatNodeAccessor::SCALE_Y:
            {
                const Vec2& scale = transform->getScale();

                values[0] = scale.y;

                break;
            }
            case FlatNodeAccessor::SCALE_XY:
            {
                const Vec2& scale = transform->getScale();

                values[0] = scale.x;
                values[1] = scale.y;

                break;
            }
            case FlatNodeAccessor::ANCHOR_X:
            {
                const Vec2& anchor = transform->getAnchorPoint();

                values[0] = anchor.x;

                break;
            }
            case FlatNodeAccessor::ANCHOR_Y:
            {
                const Vec2& anchor = transform->getAnchorPoint();

                values[0] = anchor.y;

                break;
            }
            case FlatNodeAccessor::ANCHOR_XY:
            {
                const Vec2& anchor = transform->getAnchorPoint();

                values[0] = anchor.x;
                values[1] = anchor.y;

                break;
            }
            case FlatNodeAccessor::ROTATION:
            {
                values[0] = transform->getRotation();

                break;
            }
            case FlatNodeAccessor::COLOR:
            {
                const Color& color = hierColor->getColor();

                values[0] = color.r;
                values[1] = color.g;
                values[2] = color.b;
                values[3] = color.a;

                break;
            }
            case FlatNodeAccessor::ALPHA:
            {
                values[0] = hierColor->getAlpha();

                break;
            }
            case FlatNodeAccessor::OPACITY:
            {
                values[0] = hierColor->getAlpha() * 255;

                break;
            }
            }
        }

        return 1;
    };

    vector<int> types {
                FlatNodeAccessor::POSITION_X,
                FlatNodeAccessor::POSITION_Y,
                FlatNodeAccessor::POSITION_XY,
                FlatNodeAccessor::SCALE,
                FlatNodeAccessor::SCALE_X,
                FlatNodeAccessor::SCALE_Y,
                FlatNodeAccessor::SCALE_XY,
                FlatNodeAccessor::ANCHOR_X,
                FlatNodeAccessor::ANCHOR_Y,
                FlatNodeAccessor::ANCHOR_XY,
                FlatNodeAccessor::ROTATION,
                FlatNodeAccessor::COLOR,
                FlatNodeAccessor::ALPHA,
                FlatNodeAccessor::OPACITY};

    TweenEngine::Tween::registerAccessor(types, func);

    return;
}

}
