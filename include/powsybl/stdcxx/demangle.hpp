/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_DEMANGLE_HPP
#define POWSYBL_STDCXX_DEMANGLE_HPP

#include <string>
#include <typeinfo>

namespace stdcxx {

std::string demangle(const char* name);

template <typename T>
std::string demangle() {
    return demangle(typeid(T).name());
}

template <typename T>
std::string demangle(const T& t) {
    return demangle(typeid(t).name());
}

std::string simpleClassName(const char* className);

template <typename T>
std::string simpleClassName() {
    return simpleClassName(typeid(T).name());
}

template <typename T>
std::string simpleClassName(const T& t) {
    return simpleClassName(typeid(t).name());
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_DEMANGLE_HPP
