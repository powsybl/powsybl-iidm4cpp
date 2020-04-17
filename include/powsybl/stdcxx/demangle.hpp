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

#include <boost/core/demangle.hpp>

namespace stdcxx {

std::string demangle(const char* name);

template <typename T>
std::string demangle();

template <typename T>
std::string demangle(const T& type);

template <>
std::string demangle(const std::type_info& type);

std::string getTypeIdName(const std::string& typeIdName);

std::string simpleClassName(const char* className);

template <typename T>
std::string simpleClassName();

template <typename T>
std::string simpleClassName(const T& type);

}  // namespace stdcxx

#include <powsybl/stdcxx/demangle.hxx>

#endif  // POWSYBL_STDCXX_DEMANGLE_HPP
