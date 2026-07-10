/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyModel.hpp>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

TopologyModel::TopologyModel(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
    
}

const Network& TopologyModel::getNetwork() const {
    return m_voltageLevel.getNetwork();
}
Network& TopologyModel::getNetwork() {
    return m_voltageLevel.getNetwork();
}
const Network& TopologyModel::getParentNetwork() const {
    return m_voltageLevel.getParentNetwork();
}
Network& TopologyModel::getParentNetwork() {
    return m_voltageLevel.getParentNetwork();
}
std::string TopologyModel::getSubnetworkId() const {
    return m_voltageLevel.getSubnetworkId();
}

VoltageLevel& TopologyModel::getVoltageLevel() {
    return m_voltageLevel;
}
const VoltageLevel& TopologyModel::getVoltageLevel() const {
    return m_voltageLevel;
}

void TopologyModel::addNextTerminals(Terminal& otherTerminal, TerminalSet& nextTerminals) {
    Connectable& otherConnectable = otherTerminal.getConnectable();
    if (stdcxx::isInstanceOf<Branch>(otherConnectable)) {
        auto& branch = dynamic_cast<Branch&>(otherConnectable);
        if (stdcxx::areSame(branch.getTerminal1(), otherTerminal)) {
            nextTerminals.emplace(branch.getTerminal2());
        } else if (stdcxx::areSame(branch.getTerminal2(), otherTerminal)) {
            nextTerminals.emplace(branch.getTerminal1());
        } else {
            throw AssertionError("Terminal is not one of the branch terminals");
        }
    } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(otherConnectable)) {
        auto& ttc = dynamic_cast<ThreeWindingsTransformer&>(otherConnectable);
        if (stdcxx::areSame(ttc.getLeg1().getTerminal(), otherTerminal)) {
            nextTerminals.emplace(ttc.getLeg2().getTerminal());
            nextTerminals.emplace(ttc.getLeg3().getTerminal());
        } else if (stdcxx::areSame(ttc.getLeg2().getTerminal(), otherTerminal)) {
            nextTerminals.emplace(ttc.getLeg1().getTerminal());
            nextTerminals.emplace(ttc.getLeg3().getTerminal());
        } else if (stdcxx::areSame(ttc.getLeg3().getTerminal(), otherTerminal)) {
            nextTerminals.emplace(ttc.getLeg1().getTerminal());
            nextTerminals.emplace(ttc.getLeg2().getTerminal());
        } else {
            throw AssertionError("Terminal is not one of the 3 legs terminals");
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
