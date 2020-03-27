/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UNDIRECTEDGRAPHRANGES_HPP
#define POWSYBL_IIDM_UNDIRECTEDGRAPHRANGES_HPP

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace math {

namespace graph {

template <typename T>
struct range_traits {

    using const_range = typename stdcxx::const_range<stdcxx::Reference<T> >;

    using range = typename stdcxx::range<stdcxx::Reference<T> >;
};

template <>
struct range_traits<unsigned long> {

    using const_range = boost::any_range<unsigned long, boost::forward_traversal_tag, const unsigned long&, std::ptrdiff_t>;

    using range = boost::any_range<unsigned long, boost::forward_traversal_tag, unsigned long&, std::ptrdiff_t>;
};

}  // namespace graph

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UNDIRECTEDGRAPHRANGES_HPP
