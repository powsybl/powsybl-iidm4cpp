/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "MultipleVariantContext.hpp"

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

MultipleVariantContext::MultipleVariantContext(unsigned long index) :
    m_index(index) {
}

unsigned long MultipleVariantContext::getVariantIndex() const {
    if (!m_index) {
        throw PowsyblException("Variant index not set");
    }
    return *m_index;
}

bool MultipleVariantContext::isIndexSet() const {
    return static_cast<bool>(m_index);
}

void MultipleVariantContext::resetIfVariantIndexIs(unsigned long index) {
    if (m_index == index) {
        m_index.reset();
    }
}

void MultipleVariantContext::setVariantIndex(unsigned long index) {
    m_index = index;
}

}  // namespace iidm

}  // namespace powsybl
