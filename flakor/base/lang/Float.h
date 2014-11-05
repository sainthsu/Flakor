#ifndef _FK_FLOAT_H_
#define _FK_FLOAT_H_

#include "Object.h"

FLAKOR_NS_BEGIN

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
    virtual void acceptVisitor(DataVisitor &visitor) { visitor.visit(this); }

private:
    float m_fValue;
};

// end of data_structure group
/// @}

FLAKOR_NS_END

#endif /* _FK_FLOAT_H_ */
