/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/demangle.hpp>

#include <boost/core/demangle.hpp>

#include <functional>
#include <memory>

namespace stdcxx {

std::string demangle(const char* name) {
    //return boost::core::demangle(name);
#if defined __GNUC__
    return boost::core::demangle(name);
#else 
    // under windows typeid(T).name() does not return a mangled name, but returns :
    //  - "class T" if T is a class
    //  - "struct T" if T is a struct
    //  - "enum T" if T is an enum
    //  - "union T" if T is an union
    // so remove this useless prefix by removing everything found before the last ' ' character
    std::string tmpString = name;
    std::size_t spaceIndex = tmpString.rfind(" ");
    if (spaceIndex != std::string::npos) {
        std::size_t pos = spaceIndex + 1;
        tmpString = tmpString.substr(pos, tmpString.size() - pos);
    }
    return boost::core::demangle(tmpString.c_str());
#endif
}

template <>
std::string demangle(const std::type_info& type) {
    return demangle(type.name());
}

std::string simpleClassName(const char* className) {
    const std::string& strClassName = demangle(className);
    std::size_t index = strClassName.find_last_of("::");

    return (index == std::string::npos) ? strClassName : strClassName.substr(index + 1, strClassName.size());
}

template <>
std::string simpleClassName(const std::type_info& type) {
    return simpleClassName(type.name());
}

}  // namespace stdcxx
