/****************************************************************************
Copyright (c) 2013-2014 Saint Hsu

http://www.flakor.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "targetMacros.h"
#include "2d/TextureRegion.h"

FLAKOR_NS_BEGIN

// implementation of TextureRegion

TextureRegion* TextureRegion::create(const std::string& filename, const Rect& rect)
{
    TextureRegion *TextureRegion = new (std::nothrow) TextureRegion();
    TextureRegion->initWithTextureFilename(filename, rect);
    TextureRegion->autorelease();

    return TextureRegion;
}

TextureRegion* TextureRegion::createWithTexture(Texture2D *texture, const Rect& rect)
{
    TextureRegion *TextureRegion = new (std::nothrow) TextureRegion();
    TextureRegion->initWithTexture(texture, rect);
    TextureRegion->autorelease();
    
    return TextureRegion;
}

TextureRegion* TextureRegion::createWithTexture(Texture2D* texture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    TextureRegion *TextureRegion = new (std::nothrow) TextureRegion();
    TextureRegion->initWithTexture(texture, rect, rotated, offset, originalSize);
    TextureRegion->autorelease();

    return TextureRegion;
}

TextureRegion* TextureRegion::create(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    TextureRegion *TextureRegion = new (std::nothrow) TextureRegion();
    TextureRegion->initWithTextureFilename(filename, rect, rotated, offset, originalSize);
    TextureRegion->autorelease();

    return TextureRegion;
}

TextureRegion::TextureRegion(void)
: _rotated(false)
, _texture(nullptr)
{
    
}

bool TextureRegion::initWithTexture(Texture2D* texture, const Rect& rect)
{
    Rect rectInPixels = FK_RECT_POINTS_TO_PIXELS(rect);
    return initWithTexture(texture, rectInPixels, false, PointZero, rectInPixels.size);
}

bool TextureRegion::initWithTextureFilename(const std::string& filename, const Rect& rect)
{
    Rect rectInPixels = FK_RECT_POINTS_TO_PIXELS( rect );
    return initWithTextureFilename(filename, rectInPixels, false, PointZero, rectInPixels.size);
}

bool TextureRegion::initWithTexture(Texture2D* texture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    _texture = texture;

    if (texture)
    {
        texture->retain();
    }

    _rectInPixels = rect;
    _rect = FK_RECT_PIXELS_TO_POINTS(rect);
    _offsetInPixels = offset;
    _offset = FK_POINT_PIXELS_TO_POINTS( _offsetInPixels );
    _originalSizeInPixels = originalSize;
    _originalSize = FK_SIZE_PIXELS_TO_POINTS( _originalSizeInPixels );
    _rotated = rotated;

    return true;
}

bool TextureRegion::initWithTextureFilename(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    _texture = nullptr;
    _textureFilename = filename;
    _rectInPixels = rect;
    _rect = FK_RECT_PIXELS_TO_POINTS( rect );
    _offsetInPixels = offset;
    _offset = FK_POINT_PIXELS_TO_POINTS( _offsetInPixels );
    _originalSizeInPixels = originalSize;
    _originalSize = FK_SIZE_PIXELS_TO_POINTS( _originalSizeInPixels );
    _rotated = rotated;

    return true;
}

TextureRegion::~TextureRegion(void)
{
    FKLOGINFO("deallocing TextureRegion: %p", this);
    FK_SAFE_RELEASE(_texture);
}

TextureRegion* TextureRegion::clone() const
{
	// no copy constructor	
    TextureRegion *copy = new (std::nothrow) TextureRegion();
    copy->initWithTextureFilename(_textureFilename.c_str(), _rectInPixels, _rotated, _offsetInPixels, _originalSizeInPixels);
    copy->setTexture(_texture);
    copy->autorelease();
    return copy;
}

void TextureRegion::setRect(const Rect& rect)
{
    _rect = rect;
    _rectInPixels = FK_RECT_POINTS_TO_PIXELS(_rect);
}

void TextureRegion::setRectInPixels(const Rect& rectInPixels)
{
    _rectInPixels = rectInPixels;
    _rect = FK_RECT_PIXELS_TO_POINTS(rectInPixels);
}

const Point& TextureRegion::getOffset() const
{
    return _offset;
}

void TextureRegion::setOffset(const Point& offsets)
{
    _offset = offsets;
    _offsetInPixels = FK_POINT_POINTS_TO_PIXELS( _offset );
}

const Point& TextureRegion::getOffsetInPixels() const
{
    return _offsetInPixels;
}

void TextureRegion::setOffsetInPixels(const Point& offsetInPixels)
{
    _offsetInPixels = offsetInPixels;
    _offset = FK_POINT_PIXELS_TO_POINTS( _offsetInPixels );
}

void TextureRegion::setTexture(Texture2D * texture)
{
    if( _texture != texture ) {
        FK_SAFE_RELEASE(_texture);
        FK_SAFE_RETAIN(texture);
        _texture = texture;
    }
}

Texture2D* TextureRegion::getTexture(void)
{
    if( _texture ) {
        return _texture;
    }

    if( _textureFilename.length() > 0 ) {
        return Director::getInstance()->getTextureCache()->addImage(_textureFilename.c_str());
    }
    // no texture or texture filename
    return NULL;
}

FLAKOR_NS_END

