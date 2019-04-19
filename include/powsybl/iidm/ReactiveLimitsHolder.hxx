/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REACTIVELIMITSHOLDER_HXX
#define POWSYBL_IIDM_REACTIVELIMITSHOLDER_HXX

#include <powsybl/iidm/ReactiveLimitsHolder.hpp>

#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
const T& ReactiveLimitsHolder::getReactiveLimits() const {
    assert(m_reactiveLimits);
    if (stdcxx::isInstanceOf<T>(m_reactiveLimits)) {
        return dynamic_cast<const T&>(*m_reactiveLimits);
    }

    const auto& validable = dynamic_cast<const Validable&>(*this);
    throw ValidationException(validable, logging::format("Incorrect reactive limits type %1%, expected %2%", stdcxx::simpleClassName<T>(), stdcxx::simpleClassName(*m_reactiveLimits)));
}

template <typename T, typename>
T& ReactiveLimitsHolder::getReactiveLimits() {
    return const_cast<T&>(static_cast<const ReactiveLimitsHolder*>(this)->getReactiveLimits<T>());
}

}  // namespace iidm

}  // namespace powsybl

#endif //POWSYBL_IIDM_REACTIVELIMITSHOLDER_HXX
