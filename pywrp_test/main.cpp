#include <iostream>
#include <string>
#include "pywrp/py.h"

int main() {
    py::Python python;
    
    auto os(python.import("os"));
    auto pardir(os->get<std::string>("pardir"));
    auto files(os->call<py::ObjectPtrList>("listdir", pardir));
    for (auto file = files.begin() ; file != files.end() ; ++file) {
        std::cout << "file: " << file->get()->asString() << std::endl;
    }

    return 0;
}
