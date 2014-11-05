#ifndef _FK_DATAVISITOR_H_
#define _FK_DATAVISITOR_H_


#include <string>

FLAKOR_NS_BEGIN

class Object;
class Bool;
class Integer;
class Float;
class Double;
class String;
class Array;
class Dictionary;
class Set;

class DataVisitor
{
public:
    virtual ~DataVisitor() {}

    /** default method, called from non-overloaded methods and for unrecognized objects */
    virtual void visitObject(const Object *p) = 0;

    virtual void visit(const Bool *p);
    virtual void visit(const Integer *p);
    virtual void visit(const Float *p);
    virtual void visit(const Double *p);
    virtual void visit(const String *p);
    virtual void visit(const Array *p);
    virtual void visit(const Dictionary *p);
    virtual void visit(const Set *p);
};

FLAKOR_NS_END

#endif
