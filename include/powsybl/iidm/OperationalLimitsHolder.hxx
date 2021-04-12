/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITSHOLDER_HXX
#define POWSYBL_IIDM_OPERATIONALLIMITSHOLDER_HXX

#include <powsybl/iidm/OperationalLimitsHolder.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
stdcxx::CReference<T> OperationalLimitsHolder::getOperationalLimits(const LimitType& type) const {
    auto it = m_operationalLimits.find(type);
    if (it == m_operationalLimits.end()) {
        return stdcxx::cref<T>();
    }
    if (!it->second) {
        return stdcxx::cref<T>();
    }
    if (stdcxx::isInstanceOf<T>(*it->second)) {
        return stdcxx::cref(*dynamic_cast<T*>(it->second.get()));
    }
    throw AssertionError(stdcxx::format("Unexpected class for operational limits of type %1%. Expected: %2%, actual: %3", type, stdcxx::demangle(*it->second), stdcxx::demangle<T>()));
}

template <typename T>
stdcxx::Reference<T> OperationalLimitsHolder::getOperationalLimits(const LimitType& type) {
    return stdcxx::ref(const_cast<const OperationalLimitsHolder*>(this)->getOperationalLimits<const T>(type));
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSHOLDER_HXX
