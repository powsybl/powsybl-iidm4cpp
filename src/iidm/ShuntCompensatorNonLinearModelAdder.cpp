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

ShuntCompensatorNonLinearModelAdder::SectionAdder::SectionAdder(ShuntCompensatorNonLinearModelAdder& parent) :
    m_parent(parent) {
}

ShuntCompensatorNonLinearModelAdder::SectionAdder::SectionAdder(ShuntCompensatorNonLinearModelAdder& parent, double b, double g) :
    m_parent(parent),
    m_b(b),
    m_g(g) {
}

ShuntCompensatorNonLinearModelAdder& ShuntCompensatorNonLinearModelAdder::SectionAdder::endSection() {
    checkBPerSection(m_parent.getShuntCompensatorAdder(), m_b);
    if (std::isnan(m_g)) {
        if (m_parent.getSectionAdders().empty()) {
            m_g = 0.0;
        } else {
            m_g = m_parent.getSectionAdders().back().m_g;
        }
    }
    m_parent.getSectionAdders().push_back(*this);
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

ShuntCompensatorNonLinearModelAdder::ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent) :
    m_parent(parent) {
}

ShuntCompensatorAdder& ShuntCompensatorNonLinearModelAdder::add() {
    if (m_sectionAdders.empty()) {
        throw ValidationException(m_parent, "a shunt compensator must have at least one section");
    }
    m_parent.setShuntCompensatorModelBuilder(stdcxx::move_to_unique(this));
    return m_parent;
}

ShuntCompensatorNonLinearModelAdder::SectionAdder ShuntCompensatorNonLinearModelAdder::beginSection() {
    return ShuntCompensatorNonLinearModelAdder::SectionAdder(*this);
}

std::unique_ptr<ShuntCompensatorModel> ShuntCompensatorNonLinearModelAdder::build() const {
    std::vector<ShuntCompensatorNonLinearModel::Section> sections;
    sections.reserve(m_sectionAdders.size());
    for (const auto& adder : m_sectionAdders) {
        sections.emplace_back(ShuntCompensatorNonLinearModel::Section(adder.m_b, adder.m_g));
    }

    return stdcxx::make_unique<ShuntCompensatorNonLinearModel>(std::move(sections));
}

unsigned long ShuntCompensatorNonLinearModelAdder::getMaximumSectionCount() const {
    return m_sectionAdders.size();
}

std::vector<ShuntCompensatorNonLinearModelAdder::SectionAdder>& ShuntCompensatorNonLinearModelAdder::getSectionAdders() {
    return m_sectionAdders;
}

const ShuntCompensatorAdder& ShuntCompensatorNonLinearModelAdder::getShuntCompensatorAdder() const {
    return m_parent;
}

}  // namespace shunt_compensator

}  // namespace iidm

}  // namespace powsybl
