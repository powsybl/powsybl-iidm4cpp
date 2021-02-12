/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreadLocalMultipleVariantContext.hpp>

#include <thread>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

thread_local stdcxx::optional<unsigned long> ThreadLocalMultipleVariantContext::m_index;

unsigned long ThreadLocalMultipleVariantContext::getVariantIndex() const {
    if (m_index) {
        return *m_index;
    }
    throw PowsyblException(stdcxx::format("Variant index not set for current thread %1%", std::this_thread::get_id()));
}

bool ThreadLocalMultipleVariantContext::isIndexSet() const {
    return static_cast<bool>(m_index);
}

void ThreadLocalMultipleVariantContext::reset() {
    m_index.reset();
}

void ThreadLocalMultipleVariantContext::resetIfVariantIndexIs(unsigned long index) {
    if (m_index && m_index == index) {
        m_index.reset();
    }
}

void ThreadLocalMultipleVariantContext::setVariantIndex(unsigned long index) {
    m_index = index;
}

}  // namespace iidm

}  // namespace powsybl
