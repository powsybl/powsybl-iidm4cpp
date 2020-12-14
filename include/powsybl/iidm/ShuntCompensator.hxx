/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATOR_HXX
#define POWSYBL_IIDM_SHUNTCOMPENSATOR_HXX

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ValidationException.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
const T& ShuntCompensator::getModel() const {
    if (!m_model) {
        throw ValidationException(*this, "shunt compensator model is null");
    }
    if (stdcxx::isInstanceOf<T>(m_model)) {
        return *dynamic_cast<const T*>(m_model.get());
    }
    throw ValidationException(*this, stdcxx::format("incorrect shunt compensator model type %1%, expected %2%", stdcxx::demangle<T>(), stdcxx::demangle(*m_model)));
}

template <typename T, typename>
T& ShuntCompensator::getModel() {
    return const_cast<T&>(static_cast<const ShuntCompensator*>(this)->getModel<T>());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATOR_HXX
