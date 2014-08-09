#ifndef _FK_FLOAT_H_
#define _FK_FLOAT_H_

#include "Object.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 * @js NA
 * @lua NA
 */

class Float : public Object
{
public:
    Float(float v)
        : m_fValue(v) {}
    float getValue() const {return m_fValue;}

    static Float* create(float v)
    {
        Float* pRet = new Float(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }

    /* override functions */
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

private:
    float m_fValue;
};

// end of data_structure group
/// @}

NS_CC_END

#endif /* _FK_FLOAT_H_ */
