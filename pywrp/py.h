#ifndef __PYWRP_PY_H__
#define __PYWRP_PY_H__

#include <string>
#include "Object.h"

namespace py
{

class Python
{
public:
    Python(const std::string& additionalSysPath = "");
    virtual ~Python();

    ObjectPtr import(const std::string& name);
};

} // namespace py

#endif // __PYWRP_PY_H__
