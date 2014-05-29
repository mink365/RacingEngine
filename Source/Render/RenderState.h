#ifndef RENDERSTATE_H
#define RENDERSTATE_H


namespace re {

//enum {
//    Off,
//    Additive
//} BlendMode;

class RenderState
{
public:
    RenderState();

    bool getDepthTest() const;
    void setDepthTest(bool value);

private:
    bool depthTest;
};

} // namespace re

#endif // RENDERSTATE_H
