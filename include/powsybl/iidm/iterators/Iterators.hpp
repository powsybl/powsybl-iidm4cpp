/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ITERATORS_ITERATORS_HPP
#define POWSYBL_IIDM_ITERATORS_ITERATORS_HPP

#include <powsybl/iidm/bits/ObjectStore.hpp>

#include <powsybl/iidm/iterators/NetworkFastIterator.hpp>
#include <powsybl/iidm/iterators/NetworkIterator.hpp>

namespace powsybl {

namespace iidm {

class HvdcConverterStation;
class Identifiable;
class MultiVariantObject;

template <typename T>
struct iterator_traits {
    typedef NetworkFastIterator<T, Identifiables::iterator> iterator;

    typedef NetworkFastIterator<const T, Identifiables::const_iterator> const_iterator;
};

template <>
struct iterator_traits<HvdcConverterStation> {
    typedef NetworkIterator<HvdcConverterStation, IdentifiableById::iterator> iterator;

    typedef NetworkIterator<const HvdcConverterStation, IdentifiableById::const_iterator> const_iterator;
};

template <>
struct iterator_traits<Identifiable> {
    typedef NetworkIterator<Identifiable, IdentifiableById::iterator> iterator;

    typedef NetworkIterator<const Identifiable, IdentifiableById::const_iterator> const_iterator;
};

template <>
struct iterator_traits<MultiVariantObject> {
    typedef NetworkIterator<MultiVariantObject, IdentifiableById::iterator> iterator;

    typedef NetworkIterator<const MultiVariantObject, IdentifiableById::const_iterator> const_iterator;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ITERATORS_ITERATORS_HPP
