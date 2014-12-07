#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include "platform.h"

namespace re {

/**
 * <code>TestFunction</code> specifies the testing function for stencil test
 * function and alpha test function.
 *
 * <p>The functions work similarly as described except that for stencil
 * test function, the reference value given in the stencil command is
 * the input value while the reference is the value already in the stencil
 * buffer.
 */
enum class TestFunction {

    /**
     * The test always fails
     */
    Never,
    /**
     * The test succeeds if the input value is equal to the reference value.
     */
    Equal,
    /**
     * The test succeeds if the input value is less than the reference value.
     */
    Less,
    /**
     * The test succeeds if the input value is less than or equal to
     * the reference value.
     */
    LessOrEqual,
    /**
     * The test succeeds if the input value is greater than the reference value.
     */
    Greater,
    /**
     * The test succeeds if the input value is greater than or equal to
     * the reference value.
     */
    GreaterOrEqual,
    /**
     * The test succeeds if the input value does not equal the
     * reference value.
     */
    NotEqual,
    /**
     * The test always passes
     */
    Always,
};

// -------------------- DepthState -------------------------//
struct DepthState
{
    bool depthTestEnable;
    bool depthWrite;
    TestFunction function;

    bool operator==(const DepthState& right) const;
    bool operator!=(const DepthState& right) const;
};

//-------------------- BlendState ---------------------------//

/**
 * <code>BlendMode</code> specifies the blending operation to use.
 *
 */
enum class BlendMode {
    /**
     * Additive blending. For use with glows and particle emitters.
     * <p>
     * Result = Source Color + Destination Color -> (GL_ONE, GL_ONE)
     */
    Additive,
    /**
     * 预混合？当前默认的方法，在UI和model中都使用
     * Premultiplied alpha blending, for use with premult alpha textures.
     * <p>
     * Result = Source Color + (Dest Color * (1 - Source Alpha) ) -> (GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
     */
    PremultAlpha,
    /**
     * Additive blending that is multiplied with source alpha.
     * For use with glows and particle emitters.
     * <p>
     * Result = (Source Alpha * Source Color) + Dest Color -> (GL_SRC_ALPHA, GL_ONE)
     */
    AlphaAdditive,
    /**
     * Color blending, blends in color from dest color
     * using source color.
     * <p>
     * Result = Source Color + (1 - Source Color) * Dest Color -> (GL_ONE, GL_ONE_MINUS_SRC_COLOR)
     */
    Color,
    /**
     * Alpha blending, interpolates to source color from dest color
     * using source alpha.
     * <p>
     * Result = Source Alpha * Source Color +
     *          (1 - Source Alpha) * Dest Color -> (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
     */
    Alpha,
    /**
     * Multiplies the source and dest colors.
     * <p>
     * Result = Source Color * Dest Color -> (GL_DST_COLOR, GL_ZERO)
     */
    Modulate,
    /**
     * Multiplies the source and dest colors then doubles the result.
     * <p>
     * Result = 2 * Source Color * Dest Color -> (GL_DST_COLOR, GL_SRC_COLOR)
     */
    ModulateX2,
};

struct BlendState
{
    bool blendEnable;
    BlendMode blendModeRGB;
    BlendMode blendModeAlpha;

    bool operator==(const BlendState& right) const;
    bool operator!=(const BlendState& right) const;
};

// -------------------- StencilTestState ----------------------------------//

enum class StencilOperation {

    /**
     * Keep the current value.
     */
    Keep,
    /**
     * Set the value to 0
     */
    Zero,
    /**
     * Replace the value in the stencil buffer with the reference value.
     */
    Replace,

    /**
     * Increment the value in the stencil buffer, clamp once reaching
     * the maximum value.
     */
    Increment,

    /**
     * Increment the value in the stencil buffer and wrap to 0 when
     * reaching the maximum value.
     */
    IncrementWrap,
    /**
     * Decrement the value in the stencil buffer and clamp once reaching 0.
     */
    Decrement,
    /**
     * Decrement the value in the stencil buffer and wrap to the maximum
     * value when reaching 0.
     */
    DecrementWrap,

    /**
     * Does a bitwise invert of the value in the stencil buffer.
     */
    Invert
};

struct StencilState {
    bool stencilTestEnable = false;
    StencilOperation frontStencilStencilFailOperation = StencilOperation::Keep;
    StencilOperation frontStencilDepthFailOperation = StencilOperation::Keep;
    StencilOperation frontStencilDepthPassOperation = StencilOperation::Keep;
    StencilOperation backStencilStencilFailOperation = StencilOperation::Keep;
    StencilOperation backStencilDepthFailOperation = StencilOperation::Keep;
    StencilOperation backStencilDepthPassOperation = StencilOperation::Keep;
    TestFunction frontStencilFunction = TestFunction::Always;
    TestFunction backStencilFunction = TestFunction::Always;

    bool operator==(const StencilState& right) const;
    bool operator!=(const StencilState& right) const;
};

// ------------------- AlphaTestState ---------------------------------//

struct AlphaState {
    bool alphaTestEnable;
    float alphaFallOff;
    TestFunction function;

    bool operator==(const AlphaState& right) const;
    bool operator!=(const AlphaState& right) const;
};

// -------------------- FaceCullState --------------------------------//
enum class FaceCullMode {
    /**
     * Cull front faces
     */
    Front,
    /**
     * Cull back faces
     */
    Back,
    /**
     * Cull both front and back faces.
     */
    FrontAndBack
};

struct FaceCullState {
    bool faceCullEnable;
    FaceCullMode cullMode;

    bool operator==(const FaceCullState& right) const;
    bool operator!=(const FaceCullState& right) const;
};

// ---------------------- TextureUnitContextState ----------------------//

struct TextureUnitContexState {
    bool unitEnabled;
    GPU_ID textureId;
};

class RenderState
{
public:
    RenderState();

    BlendState blendState;
    DepthState depthState;
    AlphaState alphaState;
    StencilState stencilState;
    FaceCullState faceCullState;
};

} // namespace re

#endif // RENDERSTATE_H
