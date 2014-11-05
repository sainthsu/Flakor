#ifndef _FK_BOOL_H_
#define _FK_BOOL_H_

#include "base/lang/DataVisitor.h"

FLAKOR_NS_BEGIN

class Bool : public Object
{
public:
	Bool(bool v)
        : m_bValue(v) {}
    bool getValue() const {return m_bValue;}

    static Bool* create(bool v)
    {
        Bool* pRet = new Bool(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }

    /* override functions */
    virtual void acceptVisitor(DataVisitor &visitor) { visitor.visit(this); }

private:
	bool m_bValue;
};

FLAKOR_NS_END

#endif
