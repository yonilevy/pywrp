#include "Object.h"

#include <exception>

namespace py
{

ObjectPtr Object::make_ptr(::PyObject* obj, bool shouldRelease /* = true */)
{
    return ObjectPtr(new Object(obj, shouldRelease));
}

ObjectPtr Object::from(const std::string& arg, bool shouldRelease /* = true */)
{
    return make_ptr(PyString_FromString(arg.c_str()), shouldRelease);
}

ObjectPtr Object::from(long arg, bool shouldRelease /* = true */)
{
    return make_ptr(PyInt_FromLong(arg), shouldRelease);
}

Object::Object(::PyObject* obj, bool shouldRelease /* = true */)
    : m_shouldRelease(shouldRelease),
      m_obj(obj)
{
    if (m_obj == NULL) {
        // TODO:  use PyErr Occurred, PyErr_Fetch etc.
        PyErr_Print();
        throw std::exception("Python error...");
    }
}

Object::~Object()
{
    if (m_shouldRelease) {
        Py_DECREF(m_obj);
    }
}

::PyObject* Object::raw() const
{
    return m_obj;
}

::PyObject* Object::steal()
{
    m_shouldRelease = false;
    return m_obj;
}

::PyObject* Object::copy()
{
    ::Py_IncRef(m_obj);
    return m_obj;
}

long Object::asLong() const
{
    if (!PyInt_Check(m_obj)) {
        throw std::exception("Expected type to be long, well it wasn't");
    }

    return PyInt_AsLong(m_obj);
}

std::string Object::asString() const
{
    if (!PyString_Check(m_obj)) {
        throw std::exception("Expected type to be string, well it wasn't");
    }

    return PyString_AsString(m_obj);
}

std::vector<ObjectPtr> Object::asList() const
{
    if (!PyList_Check(m_obj)) {
        throw std::exception("Expected type to be list, well it wasn't");
    }

    std::vector<ObjectPtr> res;
    for (ssize_t i = 0 ; i < PyList_Size(m_obj) ; ++i) {
        ::PyObject* item(PyList_GetItem(m_obj, i));
        Py_XINCREF(item);
        res.push_back(make_ptr(item, true));
    }

    return res;
}

ObjectPtr Object::get(const std::string& name) const
{
    return make_ptr(PyObject_GetAttrString(raw(), name.c_str()));
}

ObjectPtr Object::call(const std::string& func)
{
    std::vector<ObjectPtr> vec;
    return callWithArgs(func, vec);
}

ObjectPtr Object::callWithArgs(const std::string& func, 
                               const std::vector<ObjectPtr>& args)
{
    auto pyFunc(get(func));
    if (!PyCallable_Check(pyFunc->raw())) {
        throw std::exception("Given attribute isn't callable");
    }

    Object pyArgs(PyTuple_New(args.size()));
    ssize_t i = 0;
    for (auto it = args.begin(); it != args.end() ; ++it, ++i) {
        // Note: we are stealing the argument here, as SetItem steals
        //       ownership over items.
        PyTuple_SetItem(pyArgs.raw(), i, it->get()->copy());
    }

    return make_ptr(PyObject_CallObject(pyFunc->raw(), pyArgs.raw()));
}

} // namespace py
