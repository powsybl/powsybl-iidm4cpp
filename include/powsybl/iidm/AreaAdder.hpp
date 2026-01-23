/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_AREADDER_HPP
#define POWSYBL_IIDM_AREADDER_HPP

#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class AreaBoundary;
class Boundary;
class Terminal;

class AreaAdder : public IdentifiableAdder<Area, AreaAdder>{

private:
    explicit AreaAdder(Network& network);
    explicit AreaAdder(Network& network, Network& subnetwork);

    friend class Network;
    friend class Subnetwork;

public:
    ~AreaAdder() noexcept override = default;

    Area& add() override;

    AreaAdder& setAreaType(const std::string& areaType);
    AreaAdder& setInterchangeTarget(double interchangeTarget);
    AreaAdder& addVoltageLevel(VoltageLevel& voltageLevel);
    AreaAdder& addAreaBoundary(Terminal& terminal, bool ac);
    AreaAdder& addAreaBoundary(Boundary& boundary, bool ac);


protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;
    stdcxx::Reference<Network> m_subNetworkRef;

    std::string m_areaType;
    double m_interchangeTarget = stdcxx::nan();

    std::vector<std::reference_wrapper<VoltageLevel>> m_voltageLevels;

    std::vector<std::pair<stdcxx::Reference<Terminal>, bool>> m_terminalAreaBoundariesToAdd;
    std::vector<std::pair<stdcxx::Reference<Boundary>, bool>> m_boundaryAreaBoundariesToAdd;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_AREADDER_HPP