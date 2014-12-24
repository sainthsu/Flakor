#ifndef _FK_LORDER_H_
#define _FK_LORDER_H_

#include "base/lang/String.h"

//CPU数量 *２　＋　２

class Lorder
{
    protected:
        const char* type;
    public:
        virtual bool loadData(bool async);
}

#endif
