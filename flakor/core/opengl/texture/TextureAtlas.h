//
//  TextureAtlas.h
//  flakor_lib
//
//  Created by Saint Hsu on 24/7/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#ifndef _FK_TEXTUREATLATS_
#define _FK_TEXTUREATLATS_

#include "base/lang/Object.h"

FLAKOR_NS_BEGIN

class Texture2D;

class TextureAtlas : public Object
{
public:
    TextureAtlas();
    ~TextureAtlas();
    
protected:
    /** quantity of quads that are going to be drawn */
    size_t _totalQuads;
    /** quantity of quads that can be stored with the current texture atlas size */
    size_t _capacity;
    /** Texture of the texture atlas */
    Texture2D* _texture;
    /** Quads that are going to be rendered */
    V3F_C4B_T2F_Quad* _quads;
    
};


FLAKOR_NS_END


#endif /* defined(_FK_TEXTUREATLATS_) */
