//
//  ICommand.h
//  flakor_lib
//
//  Created by Saint Hsu on 12/2/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#ifndef _FLAKOR_ICOMMAND_H_
#define _FLAKOR_ICOMMAND_H_

class ICommand
{
    enum Type {
        
    };

    virtual void run() = 0;
    virtual void reset() = 0;
};

#endif
