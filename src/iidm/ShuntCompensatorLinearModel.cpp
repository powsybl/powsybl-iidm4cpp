/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorLinearModel.hpp>

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

ShuntCompensatorLinearModel::ShuntCompensatorLinearModel(ShuntCompensator& shuntCompensator, double bPerSection, double gPerSection, unsigned long maximumSectionCount) :
    m_shuntCompensator(shuntCompensator),
    m_bPerSection(bPerSection),
    m_gPerSection(gPerSection),
    m_maximumSectionCount(maximumSectionCount) {
}

double ShuntCompensatorLinearModel::getB(unsigned long sectionCount) const {
    if (sectionCount > m_maximumSectionCount) {
        throw PowsyblException(stdcxx::format("the given count of sections in service (%1%) is strictly greater than the maximum sections count", sectionCount));
    }
    return m_bPerSection * sectionCount;
}

double ShuntCompensatorLinearModel::getBPerSection() const {
    return m_bPerSection;
}

double ShuntCompensatorLinearModel::getG(unsigned long sectionCount) const {
    if (sectionCount > m_maximumSectionCount) {
        throw PowsyblException(stdcxx::format("the given count of sections in service (%1%) is strictly greater than the maximum sections count", sectionCount));
    }
    return std::isnan(m_gPerSection) ? 0 : m_gPerSection * sectionCount;
}

double ShuntCompensatorLinearModel::getGPerSection() const {
    return m_gPerSection;
}

unsigned long ShuntCompensatorLinearModel::getMaximumSectionCount() const {
    return m_maximumSectionCount;
}

const ShuntCompensatorModelType& ShuntCompensatorLinearModel::getType() const {
    static ShuntCompensatorModelType s_type = ShuntCompensatorModelType::LINEAR;
    return s_type;
}

ShuntCompensatorLinearModel& ShuntCompensatorLinearModel::setBPerSection(double bPerSection) {
    checkLinearBPerSection(m_shuntCompensator, bPerSection);
    m_bPerSection = bPerSection;
    return *this;
}

ShuntCompensatorLinearModel& ShuntCompensatorLinearModel::setGPerSection(double gPerSection) {
    m_gPerSection = gPerSection;
    return *this;
}

ShuntCompensatorLinearModel& ShuntCompensatorLinearModel::setMaximumSectionCount(unsigned long maximumSectionCount) {
    checkSections(m_shuntCompensator, m_shuntCompensator.getSectionCount(), maximumSectionCount);
    m_maximumSectionCount = maximumSectionCount;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
