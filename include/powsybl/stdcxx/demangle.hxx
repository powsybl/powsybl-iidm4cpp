/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_DEMANGLE_HXX
#define POWSYBL_STDCXX_DEMANGLE_HXX

#include <string>

#include <powsybl/stdcxx/demangle.hpp>

namespace stdcxx {

template <typename T>
std::string demangle() {
    return demangle(getTypeIdName(typeid(T).name()).c_str());
}

template <typename T>
std::string demangle(const T& type) {
    return demangle(getTypeIdName(typeid(type).name()).c_str());
}

template <typename T>
std::string simpleClassName() {
    return simpleClassName(getTypeIdName(typeid(T).name()).c_str());
}

template <typename T>
std::string simpleClassName(const T& type) {
    return simpleClassName(getTypeIdName(typeid(type).name()).c_str());
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_DEMANGLE_HXX
