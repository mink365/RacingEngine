#include "TextStuffer.h"

#include "UI/Base/QuadStuffer.h"

#include <stack>

namespace re {

TextStuffer::TextStuffer()
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

    Pen pen;
    Markup markup;

    for (auto span : spans) {
        this->tagStackToMarkup(span->stack, markup);

        AddText(pen, markup, plain, span->start, span->end);
    }
}

void TextStuffer::AddText(Pen &pen, const Markup &markup, const wstring &text, size_t begin, size_t end)
{
    size_t length = end - begin;

    if (end == 0) {
        end = text.size();
    }

    AddChar(pen, markup, text[0], begin);
    for (auto i = begin + 1; i < length; ++i) {
        AddChar(pen, markup, text[i], text[i - 1]);
    }
}

void TextStuffer::AddChar(Pen &pen, const Markup &markup, wchar_t current, wchar_t previous)
{
    float kerning = 0;

    Glyph::constPtr glyph = markup.font->getGlyph(current);

    kerning = glyph->getKerning(previous);
    pen.x += kerning;

    AddGlyph(pen, markup.foregroundColor, *glyph.get());
}

void TextStuffer::AddGlyph(Pen &pen, const Color &color, const Glyph &glyph)
{
    float x0, y0, x1, y1;
    float width = glyph.frame->getSize().width;
    float height = glyph.frame->getSize().height;
    x0 = pen.x + glyph.offsetX;
    y0 = pen.y - (height - glyph.offsetY);
    x1 = x0 + width;
    y1 = y0 + height;

    Rect rect(x0, y0, x1, y1);

    Rect textureRect(0, 0, width, height);

    QuadStuffer::AddOriginalQuad(rect, textureRect, color, glyph.frame, geometry);
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
            tag.color = parseColor(value, 1, value.size() - 1);
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
        return;
    } else {
        plainText.append(text, 0, end);
    }

    std::stack<Tag::ptr> stack;

    size_t index = 0;
    while (index < text.size()) {
        wchar_t c = text[index];

        if (c == L'<') {
            // the end
            if (text[index + 1] == L'/') {
                // tag end

                size_t end = text.find_first_of(L"<", index);

                if (end == string::npos) {
                    plainText.append(text, index + 1, text.size() - (index + 1));

                    assert(stack.size() == 0);

                    return;
                } else {
                    stack.top()->end = end;

                    plainText.append(text, index + 1, end - (index + 1));

                    stack.pop();
                }
            } else {
                auto tag = Tag::create();

                size_t end1 = text.find_first_of(L"/>");
                size_t end2 = text.find_first_of(L">");
                if (end1 < end2) {
                    // self end tag

                    ParseAttribute(text, index + 1, end1, *tag.get());

                    // TODO: start and end?
                } else {
                    // normal tag
                    tag->start = end2;

                    ParseAttribute(text, index + 1, end2, *tag.get());
                }

                if (stack.size() > 0) {
                    stack.top()->childrenTag.push_back(tag);
                } else {
                    tags.push_back(tag);
                }
                stack.push(tag);
            }
        } else if (c == L'>') {
            if (text[index - 1] == L'/') {
                // close the self close tag

                size_t end = text.find_first_of(L"<", index);

                if (end == string::npos) {
                    plainText.append(text, index + 1, text.size() - (index + 1));

                    return;
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
            Unfold(child, stack, flatSpans);
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
}

Color FromARGBToColor(int color) {
    Color c;
    c.r = color & 0x000000FF / 256;
    c.r = color >> 8 & 0x000000FF / 256;
    c.g = color >> 16 & 0x000000FF / 256;
    c.a = color >> 24 & 0x000000FF / 256;

    return c;
}

void TextStuffer::tagStackToMarkup(const std::vector<Tag::ptr>& stack, Markup &markup)
{
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
        }
    }
}

}
