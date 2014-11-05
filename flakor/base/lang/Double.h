#ifndef _FK_DOUBLE_H_
#define _FK_DOUBLE_H_

FLAKOR_NS_BEGIN

class Object;
class DataVisitor;

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

FLAKOR_NS_END

#endif
