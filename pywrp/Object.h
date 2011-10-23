#ifndef __PYWRP_OBJECT_H__
#define __PYWRP_OBJECT_H__

#include "Python.h"
#include <memory>
#include <vector>

namespace py
{

class Object;
typedef std::shared_ptr<Object> ObjectPtr;
typedef std::vector<ObjectPtr> ObjectPtrList;

class Object
{
public:
    /// Helper for creating a shared_ptr of a new Object.
    static ObjectPtr make_ptr(::PyObject* obj, bool shouldRelease = true);

    /// Creates Objects from different types.
    static ObjectPtr from(const std::string& arg, bool shouldRelease = true);
    static ObjectPtr from(const std::wstring& arg, bool shouldRelease = true);
    static ObjectPtr from(long arg, bool shouldRelease = true);
    template<typename T> static ObjectPtr from(const std::vector<T>& arg, bool shouldRelease = true);

public:
    Object(::PyObject* obj, bool shouldRelease = true);
    virtual ~Object();

public:
    /// Several ways of obtaining a raw PyObject.

    /// Simply returns the underlying object.
    ::PyObject* raw() const;

    /// Steals the underlying object, this object won't decrease its reference count.
    ::PyObject* steal();

    /// Copies the underlying object, this object increases the reference count.
    ::PyObject* copy();

public:
    ObjectPtr get(const std::string& name) const;
    template<typename Z> Z get(const std::string& name) const;

public:
    /// I wish I had variadic templates...
    ObjectPtr call(const std::string& func);
    template<typename A> ObjectPtr call(const std::string& func, A arg1);
    template<typename A, typename B> ObjectPtr call(const std::string& func, A arg1, B arg2);
    template<typename A, typename B, typename C> ObjectPtr call(const std::string& func, A arg1, B arg2, C arg3);

    template<typename Z> Z call(const std::string& func);
    template<typename Z, typename A> Z call(const std::string& func, A arg1);
    template<typename Z, typename A, typename B> Z call(const std::string& func, A arg1, B arg2);
    template<typename Z, typename A, typename B, typename C> Z call(const std::string& func, A arg1, B arg2, C arg3);

public:
    /**
     * Treat object as a specific type, throws if object is not of given type.
     * Note: the asList() operation adds references to the list items
     * so that they could outlive the original list.
     */
    long asLong() const;
    std::string asString() const;
    std::wstring asWString() const;
    ObjectPtrList asList() const;

    /// Templated versions of the above
    template<typename T> T as();
    template<> std::string as();
    template<> std::wstring as();
    template<> long as();
    template<> ObjectPtrList as();

private:
    ObjectPtr callWithArgs(const std::string& func, 
                           const std::vector<ObjectPtr>& args);

private:
    bool m_shouldRelease;
    ::PyObject* m_obj;
};

} // namespace py

#include "Object.inl"

#endif // __PYWRP_OBJECT_H__
