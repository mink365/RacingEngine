#include "LinearLayoutGroup.h"

namespace re
{

LinearLayoutGroup::LinearLayoutGroup()
    : _axis(Axis::Horizontal), _spacing(0)
    , _align(LinearAlign::Center)
    , _forceExpandWidth(false), _forceExpandHeight(false)
{

}

LinearLayoutGroup::~LinearLayoutGroup()
{

}

void LinearLayoutGroup::Layout()
{
    auto transform = this->getComponent<Transform2D>();
    auto childTransforms = GetChildrenTransform();

    Vec2 offset;
    if (_axis == Axis::Horizontal) {
        if (!_forceExpandHeight) {
            switch (this->_align) {
                case LinearAlign::Left:
                    offset.x = this->_padding.getLeft();
                    break;
                case LinearAlign::Center:
                    offset.x = (transform->getSize().width - _padding.getHorizontal()) / 2.0f;
                    break;
                case LinearAlign::Right:
                    offset.x = transform->getSize().width - _padding.getRight();
            }
        }
    } else {

    }

    for (auto& child : childTransforms) {

        child->setPosition(offset);

        if (_axis == Axis::Horizontal) {
            offset.x += child->getSize().width;
        } else {
            offset.y += child->getSize().height;
        }
    }
}

}

