/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_HASH_HPP
#define POWSYBL_STDCXX_HASH_HPP

#include <functional>
#include <type_traits>

namespace stdcxx {

/**
 * Define a default hash functor for enum types based on its underlying type.
 *
 * @tparam T the type of the enum
 */
template <typename T, bool = std::is_enum<T>::value>
class hash {
public:
    std::size_t operator()(const T& value) const {
        using type = typename std::underlying_type<T>::type;
        return std::hash<type>{}(static_cast<type>(value));
    }
};

/**
 * Define a default hash functor for non enum types based on std::hash
 * @tparam T the type of the class
 */
template <typename T>
class hash<T, false> {
public:
    std::size_t operator()(const T& value) const {
        return std::hash<T>{}(value);
    }
};

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_HASH_HPP
