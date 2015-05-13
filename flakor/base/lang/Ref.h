//
// Copyright (c) 2014-2015 Saint Hsu
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

#pragma once

#include "macros.h"

FLAKOR_NS_BEGIN

/// Reference count structure.
struct RefCount
{
    /// Construct.
    RefCount() :
        _refs(0),
        _weakRefs(0)
    {
    }
    
    /// Destruct.
    ~RefCount()
    {
        // Set reference counts below zero to fire asserts if this object is still accessed
        _refs = -1;
        _weakRefs = -1;
    }
    
    /// Reference count. If below zero, the object has been destroyed.
    int _refs;
    /// Weak reference count.
    int _weakRefs;
};

/// Base class for intrusively reference-counted objects. These are noncopyable and non-assignable.
class Ref
{
public:
    /// Construct. Allocate the reference count structure and set an initial self weak reference.
    Ref();
    /// Destruct. Mark as expired and also delete the reference count structure if no outside weak references exist.
    virtual ~Ref();
    
    /// Increment reference count. Can also be called outside of a SharedPtr for traditional reference counting.
    void addRef();
    /// Decrement reference count and delete self if no more references. Can also be called outside of a SharedPtr for traditional reference counting.
    void releaseRef();
    /// Return reference count.
    int getRefs() const;
    /// Return weak reference count.
    int getWeakRefs() const;
    /// Return pointer to the reference count structure.
    RefCount* getRefCountPtr() { return _refCount; }
    
private:
    /// Prevent copy construction.
    Ref(const Ref& rhs);
    /// Prevent assignment.
    Ref& operator = (const Ref& rhs);
    
    /// Pointer to the reference count structure.
    RefCount* _refCount;
};

FLAKOR_NS_END
