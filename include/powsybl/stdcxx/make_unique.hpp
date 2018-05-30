/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_MAKE_UNIQUE_HPP
#define POWSYBL_STDCXX_MAKE_UNIQUE_HPP

#include <memory>

#if __cplusplus >= 201402L

namespace stdcxx {

using std::make_unique;

}  // namespace stdcxx

#else

namespace stdcxx {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace stdcxx

#endif

#endif  // POWSYBL_STDCXX_MAKE_UNIQUE_HPP
