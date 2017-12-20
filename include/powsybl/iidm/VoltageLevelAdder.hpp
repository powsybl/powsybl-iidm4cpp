/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVELADDER_HPP
#define POWSYBL_IIDM_VOLTAGELEVELADDER_HPP

#include <powsybl/stdcxx.hpp>

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/iidm/TopologyKind.hpp>

namespace powsybl {

namespace iidm {

class Substation;
class VoltageLevel;

class VoltageLevelAdder : public IdentifiableAdder<VoltageLevelAdder> {
public:
    virtual ~VoltageLevelAdder() = default;

    VoltageLevel& add();

    VoltageLevelAdder& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevelAdder& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevelAdder& setNominalVoltage(double nominalVoltage);

    VoltageLevelAdder& setTopologyKind(const TopologyKind& topologyKind);

protected: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

    Network& getNetwork() override;

private:
    explicit VoltageLevelAdder(Substation& substation);

    friend class Substation;

private:
    Substation& m_substation;

    stdcxx::optional<double> m_highVoltageLimit;

    stdcxx::optional<double> m_lowVoltageLimit;

    stdcxx::optional<double> m_nominalVoltage;

    stdcxx::optional<TopologyKind> m_topologyKind;

};

}

}

#endif  // POWSYBL_IIDM_VOLTAGELEVELADDER_HPP
