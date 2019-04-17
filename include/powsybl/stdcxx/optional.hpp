/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_OPTIONAL_HPP
#define POWSYBL_STDCXX_OPTIONAL_HPP

#if __cplusplus >= 201703L

#include <optional>

namespace stdcxx {

template <typename T>
using optional = std::optional<T>;

}  // namespace stdcxx

#else

#include <boost/optional.hpp>

namespace stdcxx {

template <typename T>
using optional = boost::optional<T>;

}  // namespace stdcxx

#endif

#endif  // POWSYBL_STDCXX_OPTIONAL_HPP
