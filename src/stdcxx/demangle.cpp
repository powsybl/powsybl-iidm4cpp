/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/demangle.hpp>

#include <cxxabi.h>
#include <memory>

namespace stdcxx {

std::string demangle(const char* name) {
    int status = -1;

    // __cxa_demangle will allocate an output buffer we have to delete
    std::unique_ptr<char, void(*)(void*)> res(abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free);

    return status ? name : res.get();
}

template <>
std::string demangle(const std::type_info& type) {
    return demangle(type.name());
}

std::string simpleClassName(const char* className) {
    const std::string& strClassName = demangle(className);
    size_t index = strClassName.find_last_of("::");

    return (index == std::string::npos) ? strClassName : strClassName.substr(index + 1, strClassName.size());
}

template <>
std::string simpleClassName(const std::type_info& type) {
    return simpleClassName(type.name());
}

}
