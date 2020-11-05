/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_INSTANCEOF_HPP
#define POWSYBL_STDCXX_INSTANCEOF_HPP

#include <memory>

namespace stdcxx {

template <typename Base, typename T>
inline bool isInstanceOf(const T* object) {
    return dynamic_cast<const Base*>(object) != nullptr;
}

template <typename Base, typename T>
inline bool isInstanceOf(const T& object) {
    return isInstanceOf<Base, T>(&object);
}

template <typename Base, typename T>
inline bool isInstanceOf(const std::unique_ptr<T>& pointer) {
    return dynamic_cast<Base*>(pointer.get()) != nullptr;
}

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_INSTANCEOF_HPP
