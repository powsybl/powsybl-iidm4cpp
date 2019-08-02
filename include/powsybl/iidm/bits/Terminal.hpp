/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BITS_TERMINAL_HPP
#define POWSYBL_IIDM_BITS_TERMINAL_HPP

#include <powsybl/iidm/bits/Bus.hpp>

namespace powsybl {

namespace iidm {

namespace terminal {

template <typename T>
struct range_traits {

    using filter = std::function<bool(const std::reference_wrapper<Terminal>&)>;

    using const_mapper = std::function<const T&(const std::reference_wrapper<Terminal>&)>;

    using const_range = boost::transformed_range<const_mapper, const boost::filtered_range<filter, const bus::Terminals> >;

    using mapper = std::function<T&(const std::reference_wrapper<Terminal>&)>;

    using range = boost::transformed_range<mapper, const boost::filtered_range<filter, const bus::Terminals> >;
};

}  // namespace terminal

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BITS_TERMINAL_HPP
