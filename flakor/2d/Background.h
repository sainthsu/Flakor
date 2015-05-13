//
//  Background.h
//  flakor_lib
//
//  Created by Saint Hsu on 12/4/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#ifndef _FK_BACKGROUND_H_
#define _FK_BACKGROUND_H_

#include "2d/Sprite.h"

FLAKOR_NS_BEGIN

class Background : public Sprite
{
public:
    Background();
    ~Background();
protected:
    String* _src;
    Background* create();
};

FLAKOR_NS_END

#endif /* defined(_FK_BACKGROUND_H_) */
