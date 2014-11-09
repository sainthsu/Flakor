#ifndef _FK_TYPES_H_
#define _FK_TYPES_H_

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

class Double : public Object
{
public:
    Double(double v)
    : m_dValue(v) {}
    double getValue() const {return m_dValue;}
    
    static Double* create(double v)
    {
        Double* pRet = new Double(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }
    
    /* override functions */
    virtual void acceptVisitor(DataVisitor &visitor) { visitor.visit(this); }
    
private:
    double m_dValue;
};

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


FLAKOR_NS_END

#endif
