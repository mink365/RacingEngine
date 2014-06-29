#ifndef TEXTSTUFFER_H
#define TEXTSTUFFER_H

#include "Math/Vector.h"
#include "Math/Color.h"
#include "Base/Singleton.h"
#include "Base/Shared.h"

#include "Geometry/Geometry.h"

#include "Glyph.h"
#include "Font.h"

#include <stack>

namespace re {

typedef Vec2 Pen;

enum class TagType {
    Color,
    Font,
    Size,
    Bold,
    Italic,
    UnderLine,
    Image,
    Link,
};

class Tag : public Shared<Tag>
{
public:
    size_t start;
    size_t end;

    TagType type;

    union {
        int color;

        char* fontName;

        size_t fontSize;
    };

    // tag tree just like octree
    std::vector<Tag::ptr> childrenTag;
};

class Span : public Shared<Span>
{
public:
    size_t start;
    size_t end;

    std::vector<Tag::ptr> stack;
};

class Markup {
public:
    std::string family;
    float size;

    /**
     * Text color.
     */
    Color foregroundColor;

    /**
     * Background color.
     */
    Color backgroundColor;

    Font::ptr font;
};

class TextStuffer : public Singleton<TextStuffer>
{
public:
    TextStuffer();

    /**
     * @brief AddText
     * @param text
     *
     * add a rich text, text should be parse, cut to many markup block
     */
    void AddText(const std::wstring& text);
    void AddText(Pen& pen, const Markup& markup, const std::wstring& text, size_t begin=0, size_t end=0);
    void AddChar(Pen& pen, const Markup& markup, wchar_t current, wchar_t previous);
    void AddGlyph(Pen& pen, const Color& color, const Glyph& glyph);

protected:
    void parse(const wstring& text, wstring& plainText, std::vector<Tag::ptr>& tags);

    void unfold(const std::vector<Tag::ptr>& tags, std::vector<Span::ptr>& flatSpans);

    void tagStackToMarkup(const std::vector<Tag::ptr>& stack, Markup& markup);

private:
    Geometry::ptr geometry;
    Vec2 scale;
};

}

#endif // TEXTSTUFFER_H
