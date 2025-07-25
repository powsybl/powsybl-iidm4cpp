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
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

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

    VoltageLevelAdder& setNominalV(double nominalV);

    VoltageLevelAdder& setTopologyKind(const TopologyKind& topologyKind);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit VoltageLevelAdder(Network& network);

    explicit VoltageLevelAdder(Substation& substation);

    friend class Network;

    friend class Substation;

private:
    Network& m_network;

    stdcxx::Reference<Substation> m_substation;

    double m_highVoltageLimit = stdcxx::nan();

    double m_lowVoltageLimit = stdcxx::nan();

    double m_nominalV = stdcxx::nan();

    stdcxx::optional<TopologyKind> m_topologyKind;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVELADDER_HPP
