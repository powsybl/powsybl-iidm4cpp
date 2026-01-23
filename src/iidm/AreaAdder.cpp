/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AreaAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Subnetwork.hpp>

namespace powsybl {

namespace iidm {

AreaAdder::AreaAdder(Network& network) :
    m_network(network) {

}
AreaAdder::AreaAdder(Network& network, Network& subnetwork) :
    m_network(network),
    m_subNetworkRef(subnetwork) {
}

Area& AreaAdder::add() {
    std::string id = checkAndGetUniqueId();

    std::unique_ptr<Area> ptrArea;
    if (static_cast<bool>(m_subNetworkRef)) {
        ptrArea = std::unique_ptr<Area>(new Area(m_network, m_subNetworkRef, id, getName(), isFictitious(), m_areaType, m_interchangeTarget));
    } else {
        ptrArea = std::unique_ptr<Area>(new Area(m_network, id, getName(), isFictitious(), m_areaType, m_interchangeTarget));
    }
    auto& area = getNetwork().checkAndAdd<Area>(std::move(ptrArea));

    for (auto pairTerminal : m_terminalAreaBoundariesToAdd) {
        if (static_cast<bool>(pairTerminal.first)) {
            area.newAreaBoundary().setTerminal(pairTerminal.first)
                                .setAc(pairTerminal.second)
                                .add();
        }
    }
    for (auto pairBoundary : m_boundaryAreaBoundariesToAdd) {
        if (static_cast<bool>(pairBoundary.first)) {
            area.newAreaBoundary().setBoundary(pairBoundary.first)
                                .setAc(pairBoundary.second)
                                .add();
        }
    }
    for (auto vl : m_voltageLevels) {
        area.addVoltageLevel(vl);
    }

    return area;
}

AreaAdder& AreaAdder::setAreaType(const std::string& areaType) {
    m_areaType = areaType;
    return *this;
}
AreaAdder& AreaAdder::setInterchangeTarget(double interchangeTarget) {
    m_interchangeTarget = interchangeTarget;
    return *this;
}
AreaAdder& AreaAdder::addVoltageLevel(VoltageLevel& voltageLevel) {
    m_voltageLevels.emplace_back(stdcxx::ref(voltageLevel));
    return *this;
}
AreaAdder& AreaAdder::addAreaBoundary(Terminal& terminal, bool ac) {
    m_terminalAreaBoundariesToAdd.push_back(std::make_pair(stdcxx::ref(terminal), ac));
    return *this;
}
AreaAdder& AreaAdder::addAreaBoundary(Boundary& boundary, bool ac) {
    m_boundaryAreaBoundariesToAdd.push_back(std::make_pair(stdcxx::ref(boundary), ac));
    return *this;
}

const Network& AreaAdder::getNetwork() const {
    return m_network;
}
Network& AreaAdder::getNetwork() {
    return m_network;
}

const std::string& AreaAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Area";
    return s_typeDescription;
}

}  // namespace iidm

}  // namespace powsybl
