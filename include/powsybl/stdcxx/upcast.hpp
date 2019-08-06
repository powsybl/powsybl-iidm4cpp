/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_UPCAST_HPP
#define POWSYBL_STDCXX_UPCAST_HPP

#include <type_traits>

namespace stdcxx {

template <typename Derived, typename Base, typename = typename std::enable_if<std::is_base_of<Base, Derived>::value>::type>
const Base& upcast(const Derived& value) {
    return value;
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_UPCAST_HPP
