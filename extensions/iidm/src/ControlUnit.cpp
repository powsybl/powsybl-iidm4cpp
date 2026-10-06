/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ControlUnit::ControlUnit(const std::string& id, bool participate) :
    m_participate(participate) {
    if (id.empty()) {
        throw PowsyblException("Control unit id is not set");
    }
    m_id = id;
}

const std::string& ControlUnit::getId() const {
    return m_id;
}

bool ControlUnit::isParticipate() const {
    return m_participate;
}

void ControlUnit::setParticipate(bool participate) {
    m_participate = participate;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl