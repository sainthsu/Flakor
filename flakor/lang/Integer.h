#ifndef _FK_INTEGER_H_
#define _FK_INTEGER_H_

#include "Object.h"

FLAKOR_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 * @js NA
 */

class Integer : public Object
{
public:
    Integer(int v)
        : m_nValue(v) {}
    int getValue() const {return m_nValue;}

    static Integer* create(int v)
    {
        Integer* pRet = new Integer(v);
        pRet->autorelease();
        return pRet;
    }

    /* override functions 
     * @lua NA
     */
    virtual void acceptVisitor(DataVisitor &visitor) { visitor.visit(this); }

private:
    int m_nValue;
};

// end of data_structure group
/// @}

FLAKOR_CC_END

#endif /* __CCINTEGER_H__ */
