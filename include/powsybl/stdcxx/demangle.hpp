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

#include <powsybl/stdcxx/export.hpp>

namespace stdcxx {

IIDM_DECLSPEC std::string demangle(const char* name);

template <typename T>
IIDM_DECLSPEC std::string demangle() {
    return demangle(typeid(T).name());
}

template <typename T>
IIDM_DECLSPEC std::string demangle(const T& type) {
    return demangle(typeid(type).name());
}

template <>
IIDM_DECLSPEC std::string demangle(const std::type_info& type);

IIDM_DECLSPEC std::string simpleClassName(const char* className);

template <typename T>
IIDM_DECLSPEC std::string simpleClassName() {
    return simpleClassName(typeid(T).name());
}

template <typename T>
IIDM_DECLSPEC std::string simpleClassName(const T& type) {
    return simpleClassName(typeid(type).name());
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_DEMANGLE_HPP
