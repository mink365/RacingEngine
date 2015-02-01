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

void LinearLayoutGroup::CalculateLayout()
{
    auto transform = this->getComponent<Transform2D>();
    auto childTransforms = GetChildrenTransform();

    Size contentSize(transform->getSize().width - _padding.getHorizontal(),
                     transform->getSize().height - _padding.getVertical());

    Vec2 offset;
    AlignType alignType;
    if (_axis == Axis::Horizontal) {
        if (!_forceExpandHeight) {
            switch (this->_align) {
                case LinearAlign::Left:
                    offset.y = this->_padding.getTop();
                    alignType = AlignType::LEFT_TOP;
                    break;
                case LinearAlign::Center:
                    offset.y = (transform->getSize().height - _padding.getVertical()) / 2.0f;
                    alignType = AlignType::CENTER;
                    break;
                case LinearAlign::Right:
                    offset.y = transform->getSize().height - _padding.getBottom();
                    alignType = AlignType::LEFT_BOTTOM;
                    break;
            }
        } else {
            alignType = AlignType::CENTER_TOP;
        }
    } else {
        if (!_forceExpandWidth) {
            switch (this->_align) {
                case LinearAlign::Left:
                    offset.x = this->_padding.getLeft();
                    alignType = AlignType::LEFT_TOP;
                    break;
                case LinearAlign::Center:
                    offset.x = (transform->getSize().width - _padding.getHorizontal()) / 2.0f;
                    alignType = AlignType::CENTER_TOP;
                    break;
                case LinearAlign::Right:
                    offset.x = transform->getSize().width - _padding.getRight();
                    alignType = AlignType::RIGHT_TOP;
                    break;
            }
        } else {
            alignType = AlignType::LEFT_CENTER;
        }
    }

    for (auto& child : childTransforms) {

        if (_forceExpandHeight) {
            child->setSize(Size(child->getSize().width, contentSize.height));
        } else if (_forceExpandWidth) {
            child->setSize(Size(contentSize.width, child->getSize().height));
        }

        LayoutUtil::LayoutToParent(child, alignType, alignType, offset.x, offset.y);

        if (_axis == Axis::Horizontal) {
            offset.x += child->getSize().width;
        } else {
            offset.y += child->getSize().height;
        }
    }
}

}

