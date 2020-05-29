/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ENUM_HXX
#define POWSYBL_IIDM_ENUM_HXX

#include <powsybl/iidm/Enum.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <typename E, typename>
E fromString(const std::string& name) {
    const auto& names = getNames<E>();
    const auto& it = std::find(names.begin(), names.end(), name);
    if (it == names.end()) {
        throw powsybl::AssertionError(stdcxx::format("Unexpected %1% name: %2%", stdcxx::simpleClassName<E>(), name));
    }

    return static_cast<E>(it - names.begin());
}

template <typename E, typename>
std::string toString(const E& value) {
    auto index = static_cast<unsigned long>(value);
    const auto& names = getNames<E>();
    if (index >= names.size()) {
        throw powsybl::AssertionError(stdcxx::format("Unexpected %1% value: %2%", stdcxx::simpleClassName<E>(), index));
    }
    return *(names.begin() + index);
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ENUM_HXX
