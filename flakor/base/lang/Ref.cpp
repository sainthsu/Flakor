//
// Copyright (c) 2015 Saint Hsu(saint@aliyun.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "base/lang/Ref.h"

FLAKOR_NS_BEGIN

Ref::Ref() :
_refCount(new RefCount())
{
    // Hold a weak ref to self to avoid possible double delete of the refcount
    (_refCount->_weakRefs)++;
}

Ref::~Ref()
{
    FKAssert(_refCount);
    FKAssert(_refCount->_refs == 0);
    FKAssert(_refCount->_weakRefs > 0);
    
    // Mark object as expired, release the self weak ref and delete the refcount if no other weak refs exist
    refCount_->_refs = -1;
    (refCount_->_weakRefs)--;
    if (!refCount_->_weakRefs)
        delete _refCount;
    
    _refCount = 0;
}

void Ref::addRef()
{
    FKAssert(_refCount->_refs >= 0);
    (_refCount->_refs)++;
}

void Ref::releaseRef()
{
    FKAssert(_refCount->_refs > 0);
    (_refCount->_refs)--;
    if (!_refCount->_refs)
        delete this;
}

int Ref::getRefs() const
{
    return _refCount->_refs;
}

int Ref::getWeakRefs() const
{
    // Subtract one to not return the internally held reference
    return _refCount->_weakRefs - 1;
}

FLAKOR_NS_END
