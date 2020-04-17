/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_TYPEINFO_HPP
#define POWSYBL_STDCXX_TYPEINFO_HPP

#include <iosfwd>
#include <typeinfo>

#include <powsybl/stdcxx/demangle.hpp>

namespace std {

std::ostream& operator<<(std::ostream& os, const std::type_info& typeInfo) {
    os << stdcxx::demangle(typeInfo);
    return os;
}

}  // namespace std

#endif  // POWSYBL_STDCXX_TYPEINFO_HPP
