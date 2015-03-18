//
//  Engine.h
//  IOS
//
//  Created by Saint Hsu on 18/3/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#ifndef _FK_ENGINE_H_
#define _FK_ENGINE_H_

FLAKOR_NS_BEGIN

class TouchPool;
class Game;

class Engine
{
protected:
    TouchPool* pool;
    Game* game;
public:
    Engine();
};

FLAKOR_NS_END

#endif
