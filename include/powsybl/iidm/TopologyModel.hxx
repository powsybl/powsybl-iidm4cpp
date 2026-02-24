/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TOPOLOGYMODEL_HXX
#define POWSYBL_IIDM_TOPOLOGYMODEL_HXX

#include <powsybl/iidm/TopologyModel.hpp>

#include <powsybl/iidm/util/DistinctPredicate.hpp>

#include <boost/range/adaptor/filtered.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
unsigned long TopologyModel::getConnectableCount() const {
    return boost::size(getConnectables<T>());
}

template <typename T, typename>
stdcxx::const_range<T> TopologyModel::getConnectables() const {
    return getTerminals() | boost::adaptors::filtered(Terminal::isInstanceOf<T>) | boost::adaptors::transformed(Terminal::map<const T>) | boost::adaptors::filtered(DistinctPredicate());
}

template <typename T, typename>
stdcxx::range<T> TopologyModel::getConnectables() {
    return getTerminals() | boost::adaptors::filtered(Terminal::isInstanceOf<T>) | boost::adaptors::transformed(Terminal::map<T>) | boost::adaptors::filtered(DistinctPredicate());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HXX
