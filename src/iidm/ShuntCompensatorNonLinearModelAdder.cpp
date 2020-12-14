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

ShuntCompensatorNonLinearModelAdder::SectionAdder::SectionAdder(ShuntCompensatorNonLinearModelAdder& parent, const SectionAdder& adder) :
    m_parent(parent),
    m_b(adder.m_b),
    m_g(adder.m_g) {
}

ShuntCompensatorNonLinearModelAdder& ShuntCompensatorNonLinearModelAdder::SectionAdder::endSection() {
    checkbPerSection(m_parent.getShuntCompensatorAdder(), m_b);
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

ShuntCompensatorNonLinearModelAdder::ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent, const ShuntCompensatorNonLinearModelAdder& adder) :
    m_parent(parent) {
    for (const auto& sectionAdder : adder.m_sectionAdders) {
        m_sectionAdders.push_back(SectionAdder(*this, sectionAdder));
    }
}

ShuntCompensatorAdder& ShuntCompensatorNonLinearModelAdder::add() {
    if (m_sectionAdders.empty()) {
        throw ValidationException(m_parent, "a shunt compensator must have at least one section");
    }
    m_parent.setShuntCompensatorModelBuilder(*this);
    return m_parent;
}

ShuntCompensatorNonLinearModelAdder::SectionAdder ShuntCompensatorNonLinearModelAdder::beginSection() {
    return {*this};
}

std::unique_ptr<ShuntCompensatorModel> ShuntCompensatorNonLinearModelAdder::build() const {
    std::vector<ShuntCompensatorNonLinearModel::Section> sections;
    for (const auto& adder : m_sectionAdders) {
        sections.emplace_back(ShuntCompensatorNonLinearModel::Section(adder.m_b, adder.m_g));
    }

    return stdcxx::make_unique<ShuntCompensatorNonLinearModel>(std::move(sections));
}

std::unique_ptr<ShuntCompensatorModelAdder> ShuntCompensatorNonLinearModelAdder::clone(ShuntCompensatorAdder& parent) const {
    return stdcxx::make_unique<ShuntCompensatorNonLinearModelAdder>(parent, *this);
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
