#include "py.h"

#include "python.h"

namespace py
{

Python::Python(const std::string& additionalSysPath /* = "" */)
{
    Py_NoSiteFlag = 1;
    Py_Initialize();
    if (!additionalSysPath.empty()) {
        import("sys")->get("path")->call("append", additionalSysPath);
    }
}

Python::~Python()
{
    Py_Finalize();
}

ObjectPtr Python::import(const std::string& name)
{
    Object moduleName(PyString_FromString(name.c_str()));
    return Object::make_ptr(PyImport_Import(moduleName.raw()));
}

} // namespace py
