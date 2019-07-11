/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BITS_NETWORKINDEX_HPP
#define POWSYBL_IIDM_BITS_NETWORKINDEX_HPP

#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;
class MultiVariantObject;

namespace network_index {

using IdentifiableById = std::unordered_map<std::string, std::unique_ptr<Identifiable> >;

using Identifiables = std::vector<std::reference_wrapper<Identifiable> >;

using IdentifiablesByType = std::unordered_map<std::type_index, Identifiables>;

template <typename T, typename U = T>
struct range_traits {

    using const_downcaster = std::function<std::reference_wrapper<const U>(const std::reference_wrapper<Identifiable>&)>;

    using const_range = boost::transformed_range<const_downcaster, const Identifiables>;

    using downcaster = std::function<std::reference_wrapper<U>(const std::reference_wrapper<Identifiable>&)>;

    using range = boost::transformed_range<downcaster, Identifiables>;

};

template <>
struct range_traits<Identifiable, Identifiable> {

    using const_mapper = std::function<std::reference_wrapper<const Identifiable>(const IdentifiableById::value_type&)>;

    using const_range = boost::transformed_range<const_mapper, const IdentifiableById>;

    using mapper = std::function<std::reference_wrapper<Identifiable>(const IdentifiableById::value_type&)>;

    using range = boost::transformed_range<mapper, IdentifiableById>;
};

template <>
struct range_traits<Identifiable, MultiVariantObject> {

    using filter = std::function<bool(const IdentifiableById::value_type&)>;

    using const_mapper = std::function<std::reference_wrapper<const MultiVariantObject>(const IdentifiableById::value_type&)>;

    using const_range = boost::transformed_range<const_mapper, const boost::filtered_range<filter, const IdentifiableById>>;

    using mapper = std::function<std::reference_wrapper<MultiVariantObject>(const IdentifiableById::value_type&)>;

    using range = boost::transformed_range<mapper, const boost::filtered_range<filter, IdentifiableById>>;

};

}  // namespace network_index

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BITS_NETWORKINDEX_HPP
