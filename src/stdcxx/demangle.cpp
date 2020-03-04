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
    return boost::core::demangle(name);
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
