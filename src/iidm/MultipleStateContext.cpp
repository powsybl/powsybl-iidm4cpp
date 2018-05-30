/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/MultipleStateContext.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

MultipleStateContext::MultipleStateContext(unsigned long index) :
    m_index(index) {
}

unsigned long MultipleStateContext::getStateIndex() const {
    if (!m_index.is_initialized()) {
        throw PowsyblException("State not set");
    }
    return m_index.get();
}

void MultipleStateContext::resetIfStateIndexIs(unsigned long index) {
    if (m_index == index) {
        m_index.reset();
    }
}

void MultipleStateContext::setStateIndex(unsigned long index) {
    m_index = index;
}

}  // namespace iidm

}  // namespace powsybl
