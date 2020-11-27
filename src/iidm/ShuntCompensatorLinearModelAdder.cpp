/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorLinearModelAdder.hpp>

#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace shunt_compensator_view {

ShuntCompensatorLinearModelAdder::ShuntCompensatorLinearModelAdder(ShuntCompensatorAdder& parent) :
    m_parent(parent) {
}

iidm::ShuntCompensatorAdder& ShuntCompensatorLinearModelAdder::add() {
    checkLinearBPerSection(m_parent, m_bPerSection);
    checkMaximumSectionCount(m_parent, m_maximumSectionCount);
    m_parent.get().m_shuntCompensatorLinearModelAdder = *this;
    m_parent.get().m_shuntCompensatorNonLinearModelAdder.reset();
    return m_parent.get();
}

ShuntCompensatorLinearModelAdder& ShuntCompensatorLinearModelAdder::setBPerSection(double bPerSection) {
    m_bPerSection = bPerSection;
    return *this;
}

ShuntCompensatorLinearModelAdder& ShuntCompensatorLinearModelAdder::setGPerSection(double gPerSection) {
    m_gPerSection = gPerSection;
    return *this;
}

ShuntCompensatorLinearModelAdder& ShuntCompensatorLinearModelAdder::setMaximumSectionCount(unsigned long maximumSectionCount) {
    m_maximumSectionCount = maximumSectionCount;
    return *this;
}

}  // namespace shunt_compensator_view

}  // namespace iidm

}  // namespace powsybl
