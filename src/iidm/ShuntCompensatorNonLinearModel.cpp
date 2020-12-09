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

ShuntCompensatorNonLinearModel::Section::Section(ShuntCompensator& shuntCompensator, double b, double g) :
    m_shuntCompensator(shuntCompensator),
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
    checkB(m_shuntCompensator, b);
    m_b = b;
    return *this;
}

ShuntCompensatorNonLinearModel::Section& ShuntCompensatorNonLinearModel::Section::setG(double g) {
    checkG(m_shuntCompensator, g);
    m_g = g;
    return *this;
}

ShuntCompensatorNonLinearModel::ShuntCompensatorNonLinearModel(ShuntCompensator& shuntCompensator, std::vector<Section>&& sections) :
    m_shuntCompensator(shuntCompensator),
    m_sections(sections) {
}

stdcxx::const_range<ShuntCompensatorNonLinearModel::Section> ShuntCompensatorNonLinearModel::getAllSections() const {
    return m_sections;
}

stdcxx::range<ShuntCompensatorNonLinearModel::Section> ShuntCompensatorNonLinearModel::getAllSections() {
    return m_sections;
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
