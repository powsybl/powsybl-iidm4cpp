/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_MAKE_UNIQUE_HPP
#define POWSYBL_STDCXX_MAKE_UNIQUE_HPP

#include <memory>

namespace stdcxx {

template <typename B, typename D = B, typename = typename std::enable_if<std::is_base_of<B, D>::value>::type, typename... Args>
std::unique_ptr<B> make_unique(Args&&... args) {
    return std::unique_ptr<B>(new D(std::forward<Args>(args)...));
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_MAKE_UNIQUE_HPP
