#ifndef RENDERSTATE_H
#define RENDERSTATE_H

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

#endif // RENDERSTATE_H
