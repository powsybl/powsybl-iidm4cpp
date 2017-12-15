/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ITERATORS_HPP
#define POWSYBL_IIDM_ITERATORS_HPP

#include <powsybl/iidm/bits/ObjectStore.hpp>

#include <powsybl/iidm/iterators/NetworkFastIterator.hpp>
#include <powsybl/iidm/iterators/NetworkIterator.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

template <typename T>
struct iterator_traits {
    typedef NetworkFastIterator<T, IdentifiableList::iterator> iterator;

    typedef NetworkFastIterator<const T, IdentifiableList::const_iterator> const_iterator;
};

template <>
struct iterator_traits<Identifiable> {
    typedef NetworkIterator<Identifiable, MapById::iterator> iterator;

    typedef NetworkIterator<const Identifiable, MapById::const_iterator> const_iterator;
};

}

}

#endif  // POWSYBL_IIDM_ITERATORS_HPP
