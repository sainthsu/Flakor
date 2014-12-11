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

#include "base/entity/SpriteFrame.h"

FLAKOR_NS_BEGIN

// implementation of SpriteFrame

SpriteFrame* SpriteFrame::create(const std::string& filename, const Rect& rect)
{
    SpriteFrame *spriteFrame = new (std::nothrow) SpriteFrame();
    spriteFrame->initWithTextureFilename(filename, rect);
    spriteFrame->autorelease();

    return spriteFrame;
}

SpriteFrame* SpriteFrame::createWithTexture(Texture2D *texture, const Rect& rect)
{
    SpriteFrame *spriteFrame = new (std::nothrow) SpriteFrame();
    spriteFrame->initWithTexture(texture, rect);
    spriteFrame->autorelease();
    
    return spriteFrame;
}

SpriteFrame* SpriteFrame::createWithTexture(Texture2D* texture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    SpriteFrame *spriteFrame = new (std::nothrow) SpriteFrame();
    spriteFrame->initWithTexture(texture, rect, rotated, offset, originalSize);
    spriteFrame->autorelease();

    return spriteFrame;
}

SpriteFrame* SpriteFrame::create(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    SpriteFrame *spriteFrame = new (std::nothrow) SpriteFrame();
    spriteFrame->initWithTextureFilename(filename, rect, rotated, offset, originalSize);
    spriteFrame->autorelease();

    return spriteFrame;
}

SpriteFrame::SpriteFrame(void)
: _rotated(false)
, _texture(nullptr)
{
    
}

bool SpriteFrame::initWithTexture(Texture2D* texture, const Rect& rect)
{
    Rect rectInPixels = FK_RECT_POINTS_TO_PIXELS(rect);
    return initWithTexture(texture, rectInPixels, false, PointZero, rectInPixels.size);
}

bool SpriteFrame::initWithTextureFilename(const std::string& filename, const Rect& rect)
{
    Rect rectInPixels = FK_RECT_POINTS_TO_PIXELS( rect );
    return initWithTextureFilename(filename, rectInPixels, false, PointZero, rectInPixels.size);
}

bool SpriteFrame::initWithTexture(Texture2D* texture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
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

bool SpriteFrame::initWithTextureFilename(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
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

SpriteFrame::~SpriteFrame(void)
{
    FKLOGINFO("deallocing SpriteFrame: %p", this);
    FK_SAFE_RELEASE(_texture);
}

SpriteFrame* SpriteFrame::clone() const
{
	// no copy constructor	
    SpriteFrame *copy = new (std::nothrow) SpriteFrame();
    copy->initWithTextureFilename(_textureFilename.c_str(), _rectInPixels, _rotated, _offsetInPixels, _originalSizeInPixels);
    copy->setTexture(_texture);
    copy->autorelease();
    return copy;
}

void SpriteFrame::setRect(const Rect& rect)
{
    _rect = rect;
    _rectInPixels = FK_RECT_POINTS_TO_PIXELS(_rect);
}

void SpriteFrame::setRectInPixels(const Rect& rectInPixels)
{
    _rectInPixels = rectInPixels;
    _rect = FK_RECT_PIXELS_TO_POINTS(rectInPixels);
}

const Point& SpriteFrame::getOffset() const
{
    return _offset;
}

void SpriteFrame::setOffset(const Point& offsets)
{
    _offset = offsets;
    _offsetInPixels = FK_POINT_POINTS_TO_PIXELS( _offset );
}

const Point& SpriteFrame::getOffsetInPixels() const
{
    return _offsetInPixels;
}

void SpriteFrame::setOffsetInPixels(const Point& offsetInPixels)
{
    _offsetInPixels = offsetInPixels;
    _offset = FK_POINT_PIXELS_TO_POINTS( _offsetInPixels );
}

void SpriteFrame::setTexture(Texture2D * texture)
{
    if( _texture != texture ) {
        FK_SAFE_RELEASE(_texture);
        FK_SAFE_RETAIN(texture);
        _texture = texture;
    }
}

Texture2D* SpriteFrame::getTexture(void)
{
    if( _texture ) {
        return _texture;
    }

    if( _textureFilename.length() > 0 ) {
        return Director::getInstance()->getTextureCache()->addImage(_textureFilename.c_str());
    }
    // no texture or texture filename
    return nullptr;
}

FLAKOR_NS_END

