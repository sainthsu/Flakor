#include <string>

#include "base/lang/DataVisitor.h"
#include "base/lang/Array.h"
#include "base/lang/Types.h"
#include "base/lang/Dictionary.h"
#include "base/lang/Set.h"
#include "base/lang/Str.h"

FLAKOR_NS_BEGIN

void DataVisitor::visit(const Bool *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Integer *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Float *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Double *value)
{
    visitObject(value);
}

void DataVisitor::visit(const String *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Array *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Dictionary *value)
{
    visitObject(value);
}

void DataVisitor::visit(const Set *value)
{
    visitObject(value);
}



FLAKOR_NS_END
