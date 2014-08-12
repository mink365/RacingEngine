#include "PredefineTweenAccessor.h"

#include "UI/Base/Node2d.h"
#include "Tween.h"

using namespace TweenEngine;

namespace re {

void PredefineTweenAccessor::registerAccessor()
{
    auto func = [=](std::shared_ptr<void> obj, int tweenType, TweenCMD cmd, float *values)->int
    {
        Node2d* node = (Node2d*)(obj.get());

        if (cmd == TweenCMD::SET) {
            switch(tweenType) {
            case FlatNodeAccessor::POSITION_X:
            {
                node->setPositionX(values[0]);

                break;
            }
            case FlatNodeAccessor::POSITION_Y:
            {
                node->setPositionY(values[0]);

                break;
            }
            case FlatNodeAccessor::POSITION_XY:
            {
                node->setPosition(Vec2(values[0], values[1]));

                break;
            }
            case FlatNodeAccessor::SCALE:
            {
                node->setScale(values[0]);

                break;
            }
            case FlatNodeAccessor::SCALE_X:
            {
                node->setScaleX(values[0]);

                break;
            }
            case FlatNodeAccessor::SCALE_Y:
            {
                node->setScaleY(values[0]);

                break;
            }
            case FlatNodeAccessor::SCALE_XY:
            {
                node->setScale(Vec2(values[0], values[1]));

                break;
            }
            case FlatNodeAccessor::ANCHOR_X:
            {
                const Vec2& anchor = node->getAnchorPoint();

                node->setAnchorPoint(Vec2(values[0], anchor.y));

                break;
            }
            case FlatNodeAccessor::ANCHOR_Y:
            {
                const Vec2& anchor = node->getAnchorPoint();

                node->setAnchorPoint(Vec2(anchor.x, values[0]));

                break;
            }
            case FlatNodeAccessor::ANCHOR_XY:
            {
                node->setAnchorPoint(Vec2(values[0], values[1]));

                break;
            }
            case FlatNodeAccessor::ROTATION:
            {
                node->setRotation(values[0]);

                break;
            }
            case FlatNodeAccessor::COLOR:
            {
                node->setColor(Color(values[0], values[1], values[2], values[3]));

                break;
            }
            case FlatNodeAccessor::ALPHA:
            {
                node->setAlpha(values[0]);

                break;
            }
            case FlatNodeAccessor::OPACITY:
            {
                node->setAlpha(values[0] / 255);

                break;
            }
            }
        } else if (cmd == TweenCMD::GET) {
            switch(tweenType) {
            case FlatNodeAccessor::POSITION_X:
            {
                const Vec2& position = node->getPosition();

                values[0] = position.x;

                break;
            }
            case FlatNodeAccessor::POSITION_Y:
            {
                const Vec2& position = node->getPosition();

                values[0] = position.y;

                break;
            }
            case FlatNodeAccessor::POSITION_XY:
            {
                const Vec2& position = node->getPosition();

                values[0] = position.x;
                values[1] = position.y;

                break;
            }
            case FlatNodeAccessor::SCALE:
            {
                const Vec2& scale = node->getScale();

                values[0] = scale.x;

                break;
            }
            case FlatNodeAccessor::SCALE_X:
            {
                const Vec2& scale = node->getScale();

                values[0] = scale.x;

                break;
            }
            case FlatNodeAccessor::SCALE_Y:
            {
                const Vec2& scale = node->getScale();

                values[0] = scale.y;

                break;
            }
            case FlatNodeAccessor::SCALE_XY:
            {
                const Vec2& scale = node->getScale();

                values[0] = scale.x;
                values[1] = scale.y;

                break;
            }
            case FlatNodeAccessor::ANCHOR_X:
            {
                const Vec2& anchor = node->getAnchorPoint();

                values[0] = anchor.x;

                break;
            }
            case FlatNodeAccessor::ANCHOR_Y:
            {
                const Vec2& anchor = node->getAnchorPoint();

                values[0] = anchor.y;

                break;
            }
            case FlatNodeAccessor::ANCHOR_XY:
            {
                const Vec2& anchor = node->getAnchorPoint();

                values[0] = anchor.x;
                values[1] = anchor.y;

                break;
            }
            case FlatNodeAccessor::ROTATION:
            {
                values[0] = node->getRotation();

                break;
            }
            case FlatNodeAccessor::COLOR:
            {
                const Color& color = node->getColor();

                values[0] = color.r;
                values[1] = color.g;
                values[2] = color.b;
                values[3] = color.a;

                break;
            }
            case FlatNodeAccessor::ALPHA:
            {
                values[0] = node->getAlpha();

                break;
            }
            case FlatNodeAccessor::OPACITY:
            {
                values[0] = node->getAlpha() * 255;

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
