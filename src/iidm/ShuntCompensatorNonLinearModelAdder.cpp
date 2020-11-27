/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorNonLinearModelAdder.hpp>

#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace shunt_compensator_view {

ShuntCompensatorNonLinearModelAdder::ShuntCompensatorNonLinearModelAdder(ShuntCompensatorAdder& parent) :
    m_parent(parent) {
}

ShuntCompensatorNonLinearModelAdder::SectionAdder::SectionAdder(ShuntCompensatorAdder& adder, ShuntCompensatorNonLinearModelAdder& parent, unsigned long index, std::vector<ShuntCompensatorNonLinearModel::Section>& sections) :
    m_adder(adder),
    m_parent(parent),
    m_index(index),
    m_sections(sections) {
}

ShuntCompensatorNonLinearModelAdder& ShuntCompensatorNonLinearModelAdder::SectionAdder::endSection() {
    checkbPerSection(m_adder, m_b);
    if (std::isnan(m_g)) {
        if (m_sections.empty()) {
            m_g = 0.0;
        } else {
            m_g = m_sections.back().getG();
        }
    }
    m_sections.emplace_back(ShuntCompensatorNonLinearModel::Section(m_index, m_b, m_g));
    m_index++;
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
    m_parent.get().m_shuntCompensatorLinearModelAdder.reset();
    m_parent.get().m_shuntCompensatorNonLinearModelAdder = *this;
    return m_parent.get();
}

ShuntCompensatorNonLinearModelAdder::SectionAdder ShuntCompensatorNonLinearModelAdder::beginSection() {
    return {m_parent, *this, m_index, m_sections};
}

}  // namespace shunt_compensator_view

}  // namespace iidm

}  // namespace powsybl
