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

#ifndef _FK_TEXTUREREGION_H_
#define _FK_TEXTUREREGION_H_

#include "2d/Entity.h"
#include "base/element/Element.h"

FLAKOR_NS_BEGIN

class Texture2D;

/**
 * @addtogroup texture
 * @{
 */

/** @brief A TextureRegion has:
    - texture: A Texture2D that will be used by the Sprite
    - rectangle: A rectangle of the texture


 You can modify the region of a Texture to display by doing:
 
    TextureRegion *region = TextureRegion::createWithTexture(texture, rect, offset);
    sprite->setDisplayRegion(region);
 */
class TextureRegion : public Object, public Clonable
{
public:

    /** Create a TextureRegion with a texture filename, rect in points.
     It is assumed that the frame was not trimmed.
     */
    static TextureRegion* create(const std::string& filename, const Rect& rect);
    
    /** Create a TextureRegion with a texture filename, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in pixels of the frame before being trimmed.
     */
    static TextureRegion* create(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize);
    
    /** Create a TextureRegion with a texture, rect in points.
     It is assumed that the frame was not trimmed.
     */
    static TextureRegion* createWithTexture(Texture2D* pobTexture, const Rect& rect);

    /** Create a TextureRegion with a texture, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in points of the frame before being trimmed.
     */
    static TextureRegion* createWithTexture(Texture2D* pobTexture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize);

    // attributes
    inline const Rect& getRectInPixels() const { return _rectInPixels; }
    void setRectInPixels(const Rect& rectInPixels);

    inline bool isRotated(void) const { return _rotated; }
    inline void setRotated(bool rotated) { _rotated = rotated; }

    /** get rect of the frame */
    inline const Rect& getRect(void) const { return _rect; }
    /** set rect of the frame */
    void setRect(const Rect& rect);

    /** get offset of the frame */
    const Point& getOffsetInPixels(void) const;
    /** set offset of the frame */
    void setOffsetInPixels(const Point& offsetInPixels);

    /** get original size of the trimmed image */
    inline const Size& getOriginalSizeInPixels(void) const { return _originalSizeInPixels; }
    /** set original size of the trimmed image */
    inline void setOriginalSizeInPixels(const Size& sizeInPixels) { _originalSizeInPixels = sizeInPixels; }

    /** get original size of the trimmed image */
    inline const Size& getOriginalSize(void) const { return _originalSize; }
    /** set original size of the trimmed image */
    inline void setOriginalSize(const Size& sizeInPixels) { _originalSize = sizeInPixels; }

    /** get texture of the frame */
    Texture2D* getTexture(void);
    /** set texture of the frame, the texture is retained */
    void setTexture(Texture2D* pobTexture);

    const Point& getOffset(void) const;
    void setOffset(const Vec2& offsets);

    // Overrides
	virtual TextureRegion *clone() const override;
    
	protected:
    /**
     * @lua NA
     */
    TextureRegion(void);
    
    /**
     * @lua NA
     */
    virtual ~TextureRegion(void);
    
    /** Initializes a TextureRegion with a texture, rect in points.
     It is assumed that the frame was not trimmed.
     */
    bool initWithTexture(Texture2D* pobTexture, const Rect& rect);
    
    /** Initializes a TextureRegion with a texture filename, rect in points;
     It is assumed that the frame was not trimmed.
     */
    bool initWithTextureFilename(const std::string& filename, const Rect& rect);
    
    /** Initializes a TextureRegion with a texture, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in points of the frame before being trimmed.
     */
    bool initWithTexture(Texture2D* pobTexture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize);
    
    /** Initializes a TextureRegion with a texture, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in pixels of the frame before being trimmed.
     
     @since v1.1
     */
    bool initWithTextureFilename(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize);

protected:
	bool   _rotated;

	Rect _rect;
    Point _offset;
    Size _originalSize;

	Rect _rectInPixels;
    Point _offsetInPixels;
    Size _originalSizeInPixels;

    Texture2D *_texture;
    std::string  _textureFilename;
};

// end of texture group
/// @}

FLAKOR_NS_END

#endif //_FK_TextureRegion_H_
