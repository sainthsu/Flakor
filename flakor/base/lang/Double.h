#ifndef _FK_DOUBLE_H_
#define _FK_DOUBLE_H_

class Double : public Object
{
public:
	CCDouble(double v)
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
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

private:
	double m_dValue;
}

#endif
