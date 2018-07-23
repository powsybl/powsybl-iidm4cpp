/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SET_HPP
#define POWSYBL_IIDM_SET_HPP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/sequenced_index.hpp>

namespace stdcxx {

template <typename T, typename Hash = boost::hash<T>, typename Pred = std::equal_to<T> >
using LinkedHashSet = boost::multi_index_container<
    T,
    boost::multi_index::indexed_by<
        boost::multi_index::sequenced<>,
        boost::multi_index::hashed_unique<boost::multi_index::identity<T>, Hash, Pred>
    >
>;


}  // namespace stdcxx

#endif  // POWSYBL_IIDM_SET_HPP
