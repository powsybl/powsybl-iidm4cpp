/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ITERATORS_ITERATORS_HPP
#define POWSYBL_IIDM_ITERATORS_ITERATORS_HPP

#include <powsybl/iidm/bits/NetworkIndex.hpp>

#include <powsybl/iidm/iterators/NetworkFastIterator.hpp>
#include <powsybl/iidm/iterators/NetworkIterator.hpp>

namespace powsybl {

namespace iidm {

class HvdcConverterStation;
class Identifiable;
class MultiVariantObject;

template <typename T>
struct iterator_traits {
    using iterator = NetworkFastIterator<T, Identifiables::iterator>;

    using const_iterator = NetworkFastIterator<const T, Identifiables::const_iterator>;
};

template <>
struct iterator_traits<HvdcConverterStation> {
    using iterator = NetworkIterator<HvdcConverterStation, IdentifiableById::iterator>;

    using const_iterator = NetworkIterator<const HvdcConverterStation, IdentifiableById::const_iterator>;
};

template <>
struct iterator_traits<Identifiable> {
    using iterator = NetworkIterator<Identifiable, IdentifiableById::iterator>;

    using const_iterator = NetworkIterator<const Identifiable, IdentifiableById::const_iterator>;
};

template <>
struct iterator_traits<MultiVariantObject> {
    using iterator = NetworkIterator<MultiVariantObject, IdentifiableById::iterator>;

    using const_iterator = NetworkIterator<const MultiVariantObject, IdentifiableById::const_iterator>;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ITERATORS_ITERATORS_HPP
