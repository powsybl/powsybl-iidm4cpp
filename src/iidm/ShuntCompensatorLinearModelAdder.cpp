/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorLinearModelAdder.hpp>

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorLinearModel.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace shunt_compensator {

ShuntCompensatorLinearModelAdder::ShuntCompensatorLinearModelAdder(ShuntCompensatorAdder& parent) :
    ShuntCompensatorModelAdder(parent) {
}

iidm::ShuntCompensatorAdder& ShuntCompensatorLinearModelAdder::add() {
    checkLinearBPerSection(m_parent, m_bPerSection);
    checkMaximumSectionCount(m_parent, m_maximumSectionCount);
    m_parent.setShuntCompensatorModelAdder(stdcxx::make_unique<ShuntCompensatorLinearModelAdder>(*this));
    return m_parent;
}

std::unique_ptr<ShuntCompensatorModel> ShuntCompensatorLinearModelAdder::build(ShuntCompensator& shuntCompensator, unsigned long sectionCount) const {
    checkSections(m_parent, sectionCount, *m_maximumSectionCount);
    return stdcxx::make_unique<ShuntCompensatorLinearModel>(shuntCompensator, m_bPerSection, m_gPerSection, *m_maximumSectionCount);
}

std::unique_ptr<ShuntCompensatorModelAdder> ShuntCompensatorLinearModelAdder::clone() const {
    return stdcxx::make_unique<ShuntCompensatorLinearModelAdder>(*this);
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

}  // namespace shunt_compensator

}  // namespace iidm

}  // namespace powsybl
