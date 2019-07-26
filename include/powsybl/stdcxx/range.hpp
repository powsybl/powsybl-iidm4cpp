/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_RANGE_HPP
#define POWSYBL_STDCXX_RANGE_HPP

#if BOOST_VERSION < 105600

#include <iterator>

#include <boost/range/size_type.hpp>

namespace stdcxx {

template <typename R>
typename boost::range_size<R>::type size(const R& range) {
    return std::distance(boost::begin(range), boost::end(range));
}

}  // namespace stdcxx

#else

#include <boost/range/size.hpp>

namespace stdcxx {

using boost::size;

}  // namespace stdcxx

#endif  // BOOST_VERSION

#endif  // POWSYBL_IIDM_RANGE_HPP
