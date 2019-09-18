/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_RANGE_HPP
#define POWSYBL_STDCXX_RANGE_HPP

#include <boost/range/any_range.hpp>

namespace stdcxx {

template <typename T>
struct range_traits {
    using type = boost::any_range<T&, boost::forward_traversal_tag, T&, std::ptrdiff_t>;
};

template <typename T>
using range = typename range_traits<T>::type;

template <typename T>
using const_range = typename range_traits<const T>::type;

}  // namespace stdcxx

#endif  // POWSYBL_IIDM_RANGE_HPP
