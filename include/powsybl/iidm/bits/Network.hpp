/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BITS_NETWORK_HPP
#define POWSYBL_IIDM_BITS_NETWORK_HPP

#include <boost/range/join.hpp>

#include <powsybl/iidm/bits/NetworkIndex.hpp>

namespace powsybl {

namespace iidm {

class Branch;
class Line;
class MultiVariantObject;
class TieLine;
class TwoWindingsTransformer;

namespace network {

template <typename T>
struct range_traits {

    using const_range = typename network_index::range_traits<T>::const_range;

    using range = typename network_index::range_traits<T>::range;
};

template <>
struct range_traits<Branch> {

    using const_range = boost::range::joined_range<
        const boost::range::joined_range<
            const network_index::range_traits<Line, Branch>::const_range,
            const network_index::range_traits<TieLine, Branch>::const_range>,
        const network_index::range_traits<TwoWindingsTransformer, Branch>::const_range>;

    using range = boost::range::joined_range<
        const boost::range::joined_range<
            const network_index::range_traits<Line, Branch>::range,
            const network_index::range_traits<TieLine, Branch>::range>,
        const network_index::range_traits<TwoWindingsTransformer, Branch>::range>;
};

template <>
struct range_traits<HvdcConverterStation> {

    using const_range = boost::range::joined_range<
        const network_index::range_traits<LccConverterStation, HvdcConverterStation>::const_range,
        const network_index::range_traits<VscConverterStation, HvdcConverterStation>::const_range>;

    using range = boost::range::joined_range<
        const network_index::range_traits<LccConverterStation, HvdcConverterStation>::range,
        const network_index::range_traits<VscConverterStation, HvdcConverterStation>::range>;
};

template <>
struct range_traits<Line> {

    using const_range = boost::range::joined_range<
        const network_index::range_traits<Line>::const_range,
        const network_index::range_traits<TieLine, Line>::const_range>;

    using range = boost::range::joined_range<
        const network_index::range_traits<Line>::range,
        const network_index::range_traits<TieLine, Line>::range>;
};

template <>
struct range_traits<MultiVariantObject> {

    using const_range = network_index::range_traits<Identifiable, MultiVariantObject>::const_range;

    using range = network_index::range_traits<Identifiable, MultiVariantObject>::range;
};

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BITS_NETWORK_HPP
