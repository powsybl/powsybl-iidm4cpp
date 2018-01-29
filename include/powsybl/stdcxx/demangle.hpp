/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DEMANGLE_HPP
#define POWSYBL_IIDM_DEMANGLE_HPP

#include <string>
#include <typeinfo>

namespace stdcxx {

std::string demangle(const char* name);

template <typename T>
std::string demangle(const T& t) {
    return demangle(typeid(t).name());
}

}

#endif  // POWSYBL_IIDM_DEMANGLE_HPP
