/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorNonLinearModel.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

ShuntCompensatorNonLinearModel::Section::Section(double b, double g) :
    m_b(b),
    m_g(g) {
}

double ShuntCompensatorNonLinearModel::Section::getB() const {
    return m_b;
}

double ShuntCompensatorNonLinearModel::Section::getG() const {
    return m_g;
}

ShuntCompensatorNonLinearModel::Section& ShuntCompensatorNonLinearModel::Section::setB(double b) {
    checkB(m_model.get().getShuntCompensator(), b);
    m_b = b;
    return *this;
}

ShuntCompensatorNonLinearModel::Section& ShuntCompensatorNonLinearModel::Section::setG(double g) {
    checkG(m_model.get().getShuntCompensator(), g);
    m_g = g;
    return *this;
}

void ShuntCompensatorNonLinearModel::Section::setModel(ShuntCompensatorNonLinearModel& model) {
    m_model = stdcxx::ref(model);
}

ShuntCompensatorNonLinearModel::ShuntCompensatorNonLinearModel(ShuntCompensator& shuntCompensator, const std::vector<Section>& sections) :
    ShuntCompensatorModel(shuntCompensator),
    m_sections(sections) {
    for (Section& section : m_sections) {
        section.setModel(*this);
    }
}

stdcxx::const_range<ShuntCompensatorNonLinearModel::Section> ShuntCompensatorNonLinearModel::getAllSections() const {
    const auto& mapper = [](const Section& section) -> const Section& {
        return section;
    };
    return m_sections | boost::adaptors::transformed(mapper);
}

stdcxx::range<ShuntCompensatorNonLinearModel::Section> ShuntCompensatorNonLinearModel::getAllSections() {
    const auto& mapper = [](Section& section) -> Section& {
        return section;
    };
    return m_sections | boost::adaptors::transformed(mapper);
}

double ShuntCompensatorNonLinearModel::getB(unsigned long sectionCount) const {
    if (sectionCount > m_sections.size()) {
        throw ValidationException(m_shuntCompensator, "invalid section count (must be in [0;maximumSectionCount])");
    }
    return sectionCount == 0 ? 0 : m_sections[sectionCount - 1].getB();
}

double ShuntCompensatorNonLinearModel::getG(unsigned long sectionCount) const {
    if (sectionCount > m_sections.size()) {
        throw ValidationException(m_shuntCompensator, "invalid section count (must be in [0;maximumSectionCount])");
    }
    return sectionCount == 0 ? 0 : m_sections[sectionCount - 1].getG();
}

unsigned long ShuntCompensatorNonLinearModel::getMaximumSectionCount() const {
    return m_sections.size();
}

const ShuntCompensatorModelType& ShuntCompensatorNonLinearModel::getType() const {
    static ShuntCompensatorModelType s_type = ShuntCompensatorModelType::NON_LINEAR;
    return s_type;
}

}  // namespace iidm

}  // namespace powsybl
