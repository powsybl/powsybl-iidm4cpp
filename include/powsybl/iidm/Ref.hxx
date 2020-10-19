/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REF_HXX
#define POWSYBL_IIDM_REF_HXX

#include <powsybl/iidm/Ref.hpp>

#include <powsybl/AssertionError.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
Ref<T>::Ref(T& object) :
    m_pointer(&object) {
}

template <typename T>
const T& Ref<T>::get() const {
    if (m_pointer == nullptr) {
        throw AssertionError("m_pointer is null");
    }
    return *m_pointer;
}

template <typename T>
T& Ref<T>::get() {
    if (m_pointer == nullptr) {
        throw AssertionError("m_pointer is null");
    }
    return *m_pointer;
}

template <typename T>
void Ref<T>::set(T& object) {
    m_pointer = &object;
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_REF_HXX
