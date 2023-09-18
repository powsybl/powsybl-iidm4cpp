/**
 * Copyright (c) 2023, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOAD_HXX
#define POWSYBL_IIDM_LOAD_HXX

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ValidationException.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
const T& Load::getModel() const {
    if (!m_model) {
        throw ValidationException(*this, "load model is null");
    }
    if (stdcxx::isInstanceOf<T>(m_model)) {
        return *dynamic_cast<const T*>(m_model.get());
    }
    throw ValidationException(*this, stdcxx::format("incorrect load model type %1%, expected %2%", stdcxx::demangle<T>(), stdcxx::demangle(*m_model)));
}

template <typename T, typename>
T& Load::getModel() {
    return const_cast<T&>(static_cast<const Load*>(this)->getModel<T>());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOAD_HXX
