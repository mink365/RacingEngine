#ifndef RE_RECTOFFSET_H
#define RE_RECTOFFSET_H


namespace re {

class RectOffset
{
public:
    RectOffset();
    RectOffset(float left, float right, float bottom, float top);
    ~RectOffset();

    float getLeft() const;
    float getRight() const;
    float getBottom() const;
    float getTop() const;

    float getHorizontal() const;
    float getVertical() const;

    void setLeft(float v);
    void setRight(float v);
    void setBottom(float v);
    void setTop(float v);

private:
    float _left, _right, _bottom, _top;
};

} // namespace re

#endif // RE_RECTOFFSET_H
