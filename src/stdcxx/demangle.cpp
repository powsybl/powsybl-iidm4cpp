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
    std::unique_ptr<char, void(*)(void*)> res(abi::__cxa_demangle(name, nullptr, 0, &status), std::free);

    return status ? name : res.get();
}

template <>
std::string demangle(const std::type_info& type) {
    return demangle(type.name());
}

}
