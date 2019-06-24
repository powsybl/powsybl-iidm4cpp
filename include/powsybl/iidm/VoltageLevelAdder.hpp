/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVELADDER_HPP
#define POWSYBL_IIDM_VOLTAGELEVELADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Substation;
class VoltageLevel;

class VoltageLevelAdder : public IdentifiableAdder<VoltageLevelAdder> {
public:
    ~VoltageLevelAdder() noexcept override = default;

    VoltageLevel& add();

    VoltageLevelAdder& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevelAdder& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevelAdder& setNominalVoltage(double nominalVoltage);

    VoltageLevelAdder& setTopologyKind(const TopologyKind& topologyKind);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit VoltageLevelAdder(Substation& substation);

    friend class Substation;

private:
    Substation& m_substation;

    double m_highVoltageLimit;

    double m_lowVoltageLimit;

    double m_nominalVoltage;

    stdcxx::optional<TopologyKind> m_topologyKind;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVELADDER_HPP
