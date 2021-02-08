/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODEL_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODEL_HPP

#include <powsybl/iidm/ShuntCompensatorModel.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensator;

class ShuntCompensatorLinearModel : public ShuntCompensatorModel {
public:
    ShuntCompensatorLinearModel(double bPerSection, double gPerSection, unsigned long maximumSectionCount);

    ~ShuntCompensatorLinearModel() noexcept override = default;

    /**
     * Get the susceptance per section in S.
     */
    double getBPerSection() const;

    /**
     * Get the conductance per section in S.
     */
    double getGPerSection() const;

    /**
     * Set the susceptance per section in S.
     */
    ShuntCompensatorLinearModel& setBPerSection(double bPerSection);

    /**
     * Set the conductance per section in S.
     */
    ShuntCompensatorLinearModel& setGPerSection(double gPerSection);

    /**
     * Set the maximum number of sections.
     */
    ShuntCompensatorLinearModel& setMaximumSectionCount(unsigned long maximumSectionCount);

private:  // ShuntCompensatorModel
    ShuntCompensatorLinearModel& attach(ShuntCompensator& shuntCompensator) override;

    double getB(unsigned long sectionCount) const override;

    double getG(unsigned long sectionCount) const override;

    unsigned long getMaximumSectionCount() const override;

    const ShuntCompensatorModelType& getType() const override;

private:
    stdcxx::Reference<ShuntCompensator> m_shuntCompensator;

    double m_bPerSection;

    double m_gPerSection;

    unsigned long m_maximumSectionCount;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORLINEARMODEL_HPP
