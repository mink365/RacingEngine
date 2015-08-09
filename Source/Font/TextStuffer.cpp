#include "TextStuffer.h"

#include "UI/Base/QuadStuffer.h"

#include <stack>
#include <sstream>

namespace re {

TextStuffer::TextStuffer()
    : line_ascender(0), line_descender(0), line_start(0)
{
}

void TextStuffer::AddText(const wstring &text, Geometry::ptr geometry, Font::constPtr font)
{
    this->geometry = geometry;
    this->defaultFont = font;

    wstring plain;
    std::vector<Tag::ptr> tags;
    std::vector<Span::ptr> spans;

    this->parse(text, plain, tags);

    this->unfold(tags, spans);

    Pen pen = this->vertexOrigin;
    Markup markup;

    this->rect.set(0, 0, 0, 0);

    for (auto span : spans) {
        this->tagStackToMarkup(span->stack, markup);

        AddText(pen, markup, plain, span->start, span->end);
    }

    return;
}

void TextStuffer::AddText(Pen &pen, const Markup &markup, const wstring &text, size_t begin, size_t end)
{
    if (end == 0) {
        end = text.size();
    }

    AddChar(pen, markup, text[begin], 0);
    for (auto i = begin + 1; i < end; ++i) {
        AddChar(pen, markup, text[i], text[i - 1]);
    }
}

void TextStuffer::AddChar(Pen &pen, const Markup &markup, wchar_t current, wchar_t previous)
{
    float kerning = 0;

    if (current == L'\n') {
        pen.x = this->vertexOrigin.x;
        pen.y += this->line_descender;

        this->line_descender = 0;
        this->line_ascender = 0;

        this->line_start = this->geometry->getFaces().size() / 2;

        return;
    }

    float scale = markup.size / markup.font->getSize();

    if( markup.font->ascender * scale > this->line_ascender )
    {
        float y = pen.y;
        pen.y -= (markup.font->ascender * scale - this->line_ascender);
        textBufferMoveLastLine((int)(y - pen.y) );
        this->line_ascender = markup.font->ascender * scale;
    }
    if( markup.font->descender * scale < this->line_descender )
    {
        this->line_descender = markup.font->descender * scale;
    }

    Glyph::constPtr glyph = markup.font->getGlyph(current);

    kerning = glyph->getKerning(previous);
    pen.x += kerning * scale;

    this->scale.x = scale;
    this->scale.y = scale;

    AddGlyph(pen, markup.foregroundColor, *glyph.get());
}

void TextStuffer::AddGlyph(Pen &pen, const Color &color, const Glyph &glyph)
{
    float x0, y0;
    float width = glyph.getWidth() * scale.x;
    float height = glyph.getHeight() * scale.y;
    x0 = pen.x + glyph.offsetX * scale.x;
    y0 = pen.y - (glyph.getHeight() - glyph.offsetY) * scale.y;

    Rect rect(x0, y0, width, height);
    Rect textureRect(0, 0, glyph.getWidth(), glyph.getHeight());

    QuadStuffer::AddOriginalQuad(rect, textureRect, color, glyph.frame, geometry);

    pen.x += glyph.advanceX * scale.x;

    if (this->rect.getWidth() == 0) {
        this->rect = rect;
    } else {
        this->rect = this->rect.unionWithRect(rect);
    }
}

std::vector<std::wstring> &split(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems) {
    std::wstringstream ss(s);
    std::wstring item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

bool compare(const std::wstring& source, const std::wstring& text) {
    size_t index = 0;
    while(index < text.size()) {

        if (source[index] != text[index]) {
            return false;
        }

        index ++;
    }

    return true;
}

const string getValue(const wstring& text) {
    size_t p = text.find_first_of(L"=");
    wstring v = text.substr(p + 1, text.size() - (p + 1) - 1);

    return std::string(v.begin(), v.end());
}

int parseColor(const std::string& text, int start, int len) {
    int color = 0;
    int end = start + len;

    // Alpha
    if (len <= 6) {
        color <<= 4;
        color |= 15;

        color <<= 4;
        color |= 15;
    }

    for(int i = start; i < end; i++) {
        color <<= 4;
        char c = text[i];
        if(c >= '0' && c <= '9') {
            color |= c - '0';
        } else if(c >= 'a' && c <= 'f') {
            color |= c - 'a' + 10;
        } else if(c >= 'A' && c <= 'F') {
            color |= c - 'A' + 10;
        }
    }

//    printf("color Text: %s, HEx: 0x%X \n", text.c_str(), color);

    return color;
}

void ParseAttribute(const wstring& text, size_t start, size_t end, Tag& tag) {
    const wstring& str = text.substr(start, end - start);

    if (str.size() == 1) {
        if (str == L"b") {
            tag.type = TagType::Bold;
        } else if (str == L"i") {
            tag.type = TagType::Italic;
        }
    } else {
        std::vector<std::wstring> elems;
        split(str, L'=', elems);

        assert(elems.size() == 2);

        const wstring name = elems[0];
        const string value = string(elems[1].begin(), elems[1].end());
        if (compare(name, L"font")) {
            tag.type = TagType::Font;

            // TODO: "" surround the font name
            tag.fontName = new char[value.size()];
            strcpy(tag.fontName, value.c_str());

        } else if (compare(name, L"size")) {
            tag.type = TagType::Size;

            tag.fontSize = std::stoi(value);

        } else if (compare(name, L"color")) {
            tag.type = TagType::Color;
            // #FFFFFF#
            tag.color = parseColor(value, 0, value.size());
        } else {
            // some error
            assert(false);
        }
    }
}

void TextStuffer::parse(const wstring& text, wstring& plainText, std::vector<Tag::ptr>& tags)
{
    // append plain text before tag
    size_t end = text.find_first_of(L"<", 0);
    if (end == string::npos) {
        plainText.append(text, 0, text.size());

        auto rootTag = Tag::create();
        rootTag->type = TagType::Root;
        rootTag->start = 0;
        rootTag->end = plainText.size();
        tags.push_back(rootTag);

        return;
    } else {
        plainText.append(text, 0, end);
    }

    std::stack<Tag::ptr> stack;

    // add the root tag, root tag is not set by use. it just like this: <tag=...>xxx...xxx</tag>
    auto rootTag = Tag::create();
    rootTag->type = TagType::Root;
    rootTag->start = 0;
    tags.push_back(rootTag);
    stack.push(rootTag);

    size_t index = 0;
    while (index < text.size()) {
        wchar_t c = text[index];

        if (c == L'<') {
            if (text[index + 1] == L'/') {
                // tag end

                size_t start = text.find_last_of(L">", index);

                plainText.append(text, start + 1, index - (start + 1));
                stack.top()->end = plainText.size();

                stack.pop();

                size_t nextTag = text.find_first_of(L"<", index + 1);
                if (nextTag == string::npos) {
                    size_t end2 = text.find_first_of(L">", index + 1);

                    plainText.append(text, end2 + 1, text.size() - (end2 + 1));

                    break;
                }

            } else {
                // tag start

                auto tag = Tag::create();

                // push the plain text to the top tag
                size_t lastTagEnd = text.find_last_of(L">", index);
                if (lastTagEnd != string::npos) {
                    plainText.append(text, lastTagEnd + 1, index - (lastTagEnd + 1));
                }

                size_t end1 = text.find_first_of(L"/>", index + 1);
                size_t end2 = text.find_first_of(L">", index + 1);
                if (end1 < end2) {
                    // self end tag

                    ParseAttribute(text, index + 1, end1, *tag.get());

                    // TODO: start and end?
                } else {
                    // normal tag
                    tag->start = plainText.size();

                    ParseAttribute(text, index + 1, end2, *tag.get());
                }

                stack.top()->childrenTag.push_back(tag);

                stack.push(tag);
            }
        } else if (c == L'>') {
            if (text[index - 1] == L'/') {
                // close the self close tag

                size_t end = text.find_first_of(L"<", index);

                if (end == string::npos) {
                    plainText.append(text, index + 1, text.size() - (index + 1));

                    break;
                } else {
                    plainText.append(text, index + 1, end - (index + 1));
                }

                stack.pop();
            } else {
                // close the tag attribute
            }
        }

        index ++;
    }

    rootTag->end = plainText.size();

    return;
}

void Unfold(const Tag::ptr& tag, std::vector<Tag::ptr>& stack, std::vector<Span::ptr>& flatSpans) {
    stack.push_back(tag);

    if (tag->childrenTag.size() == 0) {
        auto span = Span::create();
        span->start = tag->start;
        span->end = tag->end;
        span->stack = stack;

        flatSpans.push_back(span);
    } else {
        for (auto child : tag->childrenTag) {
            size_t start = 0;

            if (flatSpans.size() > 0) {
                start = flatSpans[flatSpans.size() - 1]->end;
            } else {
                start = tag->start;
            }
            if (start < child->start) {
                auto span = Span::create();
                span->start = start;
                span->end = child->start;
                span->stack = stack;

                flatSpans.push_back(span);
            }

            Unfold(child, stack, flatSpans);
        }

        auto start = flatSpans[flatSpans.size() - 1]->end;
        if (start < tag->end) {
            auto span = Span::create();
            span->start = start;
            span->end = tag->end;
            span->stack = stack;

            flatSpans.push_back(span);
        }
    }

    stack.pop_back();
}

void TextStuffer::unfold(const std::vector<Tag::ptr> &tags, std::vector<Span::ptr>& flatSpans)
{
    flatSpans.clear();

    std::vector<Tag::ptr> stack;

    for (auto tag : tags) {
        Unfold(tag, stack, flatSpans);
    }

    return;
}

void TextStuffer::defaultMarkup(Markup &markup)
{
    markup.foregroundColor = Color::White;
    markup.backgroundColor = Color::White;
    markup.size = 25;
    markup.font = FontManager::getInstance().GetResource("default");
}

Color FromARGBToColor(int color) {
    Color c;
    c.b = color & 0xFF / 255;
    c.g = color >> 8 & 0xFF / 255;
    c.r = color >> 16 & 0xFF / 255;
    c.a = color >> 24 & 0xFF / 255;

    return c;
}

void TextStuffer::tagStackToMarkup(const std::vector<Tag::ptr>& stack, Markup &markup)
{
    this->defaultMarkup(markup);

    for (auto tag : stack) {
        switch (tag->type) {
        case TagType::Color:
        {
            markup.foregroundColor = FromARGBToColor(tag->color);

            break;
        }
        case TagType::Bold:
        {
            break;
        }
        case TagType::Font:
        {
            markup.font = FontManager::getInstance().GetResource(tag->fontName);

            break;
        }
        case TagType::Italic:
        {

            break;
        }
        case TagType::Size:
        {
            markup.size = tag->fontSize;
            break;
        }
        case TagType::Root:
        case TagType::Image:
        case TagType::UnderLine:
        case TagType::Link:
            break;
        }
    }
}

void TextStuffer::textBufferMoveLastLine(float dy)
{
    std::vector<Face>& faces = this->geometry->getFaces();
    std::vector<Vec3>& vertices = this->geometry->getPositions();

    for (size_t i=this->line_start * 2; i < faces.size(); ++i) {
        const Face& face = faces[i];

        vertices[face.a].y -= dy;
        vertices[face.b].y -= dy;
        vertices[face.c].y -= dy;
    }
}

}
