/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorNonLinearModelAdder.hpp>

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace shunt_compensator {

ShuntCompensatorNonLinearModelAdder::ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent) :
    ShuntCompensatorModelAdder(parent) {
}

ShuntCompensatorNonLinearModelAdder::SectionAdder::SectionAdder(ShuntCompensatorNonLinearModelAdder& parent, std::vector<ShuntCompensatorNonLinearModel::Section>& sections) :
    m_parent(parent),
    m_sections(sections) {
}

ShuntCompensatorNonLinearModelAdder& ShuntCompensatorNonLinearModelAdder::SectionAdder::endSection() {
    checkbPerSection(m_parent.m_parent, m_b);
    if (std::isnan(m_g)) {
        if (m_sections.empty()) {
            m_g = 0.0;
        } else {
            m_g = m_sections.back().getG();
        }
    }
    m_sections.emplace_back(ShuntCompensatorNonLinearModel::Section(m_b, m_g));
    return m_parent;
}

ShuntCompensatorNonLinearModelAdder::SectionAdder& ShuntCompensatorNonLinearModelAdder::SectionAdder::setB(double b) {
    m_b = b;
    return *this;
}

ShuntCompensatorNonLinearModelAdder::SectionAdder& ShuntCompensatorNonLinearModelAdder::SectionAdder::setG(double g) {
    m_g = g;
    return *this;
}

ShuntCompensatorAdder& ShuntCompensatorNonLinearModelAdder::add() {
    if (m_sections.empty()) {
        throw ValidationException(m_parent, "a shunt compensator must have at least one section");
    }
    m_parent.setShuntCompensatorModelAdder(stdcxx::make_unique<ShuntCompensatorNonLinearModelAdder>(*this));
    return m_parent;
}

ShuntCompensatorNonLinearModelAdder::SectionAdder ShuntCompensatorNonLinearModelAdder::beginSection() {
    return {*this, m_sections};
}

std::unique_ptr<ShuntCompensatorModel> ShuntCompensatorNonLinearModelAdder::build(ShuntCompensator& shuntCompensator, unsigned long sectionCount) const {
    checkSections(m_parent, sectionCount, m_sections.size());
    return stdcxx::make_unique<ShuntCompensatorNonLinearModel>(shuntCompensator, m_sections);
}

std::unique_ptr<ShuntCompensatorModelAdder> ShuntCompensatorNonLinearModelAdder::clone() const {
    return stdcxx::make_unique<ShuntCompensatorNonLinearModelAdder>(*this);
}

}  // namespace shunt_compensator

}  // namespace iidm

}  // namespace powsybl
