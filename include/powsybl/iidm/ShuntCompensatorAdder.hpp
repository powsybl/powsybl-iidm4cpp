/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORADDER_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>

namespace powsybl {

namespace iidm {

class Network;
class ShuntCompensator;
class VoltageLevel;

class ShuntCompensatorAdder : public InjectionAdder<ShuntCompensatorAdder> {

public:
    ~ShuntCompensatorAdder() noexcept override = default;

    ShuntCompensator& add();

    ShuntCompensatorAdder& setbPerSection(double bPerSection);

    ShuntCompensatorAdder& setCurrentSectionCount(unsigned long currentSectionCount);

    ShuntCompensatorAdder& setMaximumSectionCount(unsigned long maximumSectionCount);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit ShuntCompensatorAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    VoltageLevel& m_voltageLevel;

    double m_bPerSection;

    unsigned long m_currentSectionCount;

    unsigned long m_maximumSectionCount;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORADDER_HPP
