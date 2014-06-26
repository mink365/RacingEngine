#include "TextureAtlas.h"

#include <algorithm>
#include <limits.h>

#include "Image/Image.h"

namespace re {

void eraseNode(vector<AtlasNode*>& nodes, int index) {
    auto node = nodes[index];

    auto iter = std::find(nodes.begin(), nodes.end(), node);

    assert(iter != nodes.end());

    nodes.erase(iter);

    delete node;
}

void clearNodes(vector<AtlasNode*>& nodes) {
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];

        delete node;
    }

    nodes.clear();
}

TextureAtlas::TextureAtlas()
    : data(NULL), width(0), height(0), depth(0), used(0)
{
}

TextureAtlas::~TextureAtlas()
{
    clearNodes(this->nodes);
}

void TextureAtlas::init(Int width, Int height, Int depth)
{
    this->width = width;
    this->height = height;
    this->depth = depth;

    this->texture = std::make_shared<Texture>(width, height, 0);
    // TODO: add to textureManager

    // TODO: memory release

    AtlasNode node = {1,1,width-2};
    this->nodes.push_back(new AtlasNode(node));

    this->data = (unsigned char *)calloc( width*height*depth, sizeof(unsigned char) );

    if( this->data == NULL)
    {
        fprintf( stderr,
                 "line %d: No more memory for allocating data\n", __LINE__ );
        exit( EXIT_FAILURE );
    }
}

Rect TextureAtlas::getRegin(float width, float height)
{
    int y, best_height, best_width, best_index;
    Rect region = {0,0,width,height};

    best_height = INT_MAX;
    best_index  = -1;
    best_width = INT_MAX;
    for(size_t i=0; i<this->nodes.size(); ++i )
    {
        y = this->fit(i, width, height );
        if( y >= 0 )
        {
            const AtlasNode* node = this->nodes[i];
            if( ( (y + height) < best_height ) ||
                ( ((y + height) == best_height) && (node->width < best_width)) )
            {
                best_height = y + height;
                best_index = i;
                best_width = node->width;
                region.origin.x = node->x;
                region.origin.y = y;
            }
        }
    }

    if( best_index == -1 )
    {
        region.origin.x = -1;
        region.origin.y = -1;
        region.size.width = 0;
        region.size.height = 0;
        return region;
    }

    AtlasNode* node = new AtlasNode();
    if( node == NULL)
    {
        fprintf( stderr,
                 "line %d: No more memory for allocating data\n", __LINE__ );
        exit( EXIT_FAILURE );
    }
    node->x = region.origin.x;
    node->y = region.origin.y + height;
    node->width = width;
    this->nodes.insert(nodes.begin() + best_index, node);

    for(size_t i = best_index+1; i < this->nodes.size(); ++i)
    {
        AtlasNode* node = this->nodes[i];
        AtlasNode* prev = this->nodes[i - 1];

        if (node->x < (prev->x + prev->width) )
        {
            int shrink = prev->x + prev->width - node->x;
            node->x += shrink;
            node->width -= shrink;
            if (node->width <= 0)
            {
                eraseNode( this->nodes, i );
                --i;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    this->merge();
    this->used += width * height;
    return region;
}

void TextureAtlas::setRegin(const Rect &rect, const unsigned char *data, const Int stride)
{
    size_t i;
    size_t depth;
    size_t charsize;

    size_t x = rect.origin.x;
    size_t y = rect.origin.y;
    size_t width = rect.size.width;
    size_t height = rect.size.height;

    assert( x > 0);
    assert( y > 0);
    assert( x < (this->width-1));
    assert( (x + width) <= (this->width-1));
    assert( y < (this->height-1));
    assert( (y + height) <= (this->height-1));

    depth = this->depth;
    charsize = sizeof(char);
    for( i=0; i<height; ++i )
    {
        memcpy( this->data+((y+i)*this->width + x ) * charsize * depth,
                data + (i*stride) * charsize, width * charsize * depth  );
    }
}

void TextureAtlas::clear()
{
    AtlasNode node = {1,1,1};

    assert( this->data );

    clearNodes(this->nodes);
    this->used = 0;

    // We want a one pixel border around the whole atlas to avoid any artefact when
    // sampling texture
    node.width = this->width-2;

    this->nodes.push_back(new AtlasNode(node));
    memset( this->data, 0, this->width*this->height*this->depth );
}

Texture::ptr TextureAtlas::getTexture()
{
    return this->texture;
}

void TextureAtlas::upload()
{
    assert( this->data );

    if( !this->texture->getGlID() )
    {
        GLuint id = 0;

        glGenTextures( 1, &id );

        this->texture->setGlID(id);
    }

    glBindTexture( GL_TEXTURE_2D, this->texture->getGlID() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    if( this->depth == 4 )
    {
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height,
                      0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, this->data );
#else
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height,
                      0, GL_RGBA, GL_UNSIGNED_BYTE, self->data );
#endif
    }
    else if( this->depth == 3 )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height,
                      0, GL_RGB, GL_UNSIGNED_BYTE, this->data );
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, this->width, this->height,
                      0, GL_RED, GL_UNSIGNED_BYTE, this->data );
    }
}

Int TextureAtlas::fit(Int index, Int width, Int height) const
{
    int x, y, width_left;
    size_t i;

    AtlasNode* node = this->nodes[index];
    x = node->x;
    y = node->y;
    width_left = width;
    i = index;

    if ( (x + width) > (this->width-1) )
    {
        return -1;
    }
    y = node->y;
    while( width_left > 0 )
    {
        node = this->nodes[i];
        if( node->y > y )
        {
            y = node->y;
        }
        if( (y + height) > (this->height-1) )
        {
            return -1;
        }
        width_left -= node->width;
        ++i;
    }
    return y;
}

void re::TextureAtlas::merge()
{
    for(int i=0; i< this->nodes.size()-1; ++i )
    {
        AtlasNode* node = this->nodes[i];
        AtlasNode* next = this->nodes[i + 1];
        if( node->y == next->y )
        {
            node->width += next->width;
            eraseNode(nodes, i);

            --i;
        }
    }
}

}