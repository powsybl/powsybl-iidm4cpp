/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MEMORY_HPP
#define POWSYBL_IIDM_MEMORY_HPP

#include <memory>

namespace stdcxx {

template <typename T, typename U>
inline bool areSame(const T& object1, const U& object2) {
    return std::addressof(object1) == std::addressof(object2);
}

}  // namespace stdcxx

#endif  // POWSYBL_IIDM_MEMORY_HPP
