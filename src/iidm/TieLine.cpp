/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TieLine.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/SwitchPredicate.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/ConnectDisconnectUtil.hpp>
#include <powsybl/iidm/util/TieLineUtil.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

TieLine::TieLine(Network& network, const std::string& id, const std::string& name, bool fictitious) :
    Identifiable(id, name, fictitious),
    m_network(network) {
    m_boundaryLine1.reset();
    m_boundaryLine2.reset();
}

void TieLine::attachBoundaryLines(BoundaryLine& bl1, BoundaryLine& bl2) {
    m_boundaryLine1 = attach(bl1);
    m_boundaryLine2 = attach(bl2);
}
BoundaryLine& TieLine::attach(BoundaryLine& bl) {
    bl.setTieLine(*this);
    return bl;
}

const Network& TieLine::getNetwork() const {
    return m_network.get();
}

Network& TieLine::getNetwork() {
    return m_network.get();
}

const Network& TieLine::getParentNetwork() const {
    if(m_boundaryLine1 && m_boundaryLine2) {
        const Network& subNetwork1 = m_boundaryLine1.get().getParentNetwork();
        const Network& subNetwork2 = m_boundaryLine2.get().getParentNetwork();
        if(stdcxx::areSame(subNetwork1, subNetwork2)) {
            return subNetwork1;
        }
    } else if(m_boundaryLine1) {
        return m_boundaryLine1.get().getParentNetwork();
    } else if(m_boundaryLine2) {
        return m_boundaryLine2.get().getParentNetwork();
    }
    return getNetwork();
}

Network& TieLine::getParentNetwork() {
    return const_cast<Network&>(static_cast<const TieLine*>(this)->getParentNetwork());
}

double TieLine::getB1() const {
    return TieLineUtil::getB1(getBoundaryLine1(), getBoundaryLine2());
}

double TieLine::getB2() const {
    return TieLineUtil::getB2(getBoundaryLine1(), getBoundaryLine2());
}

double TieLine::getG1() const {
    return TieLineUtil::getG1(getBoundaryLine1(), getBoundaryLine2());
}

double TieLine::getG2() const {
    return TieLineUtil::getG2(getBoundaryLine1(), getBoundaryLine2());
}

double TieLine::getR() const {
    return TieLineUtil::getR(getBoundaryLine1(), getBoundaryLine2());
}

double TieLine::getX() const {
    return TieLineUtil::getX(getBoundaryLine1(), getBoundaryLine2());
}


const BoundaryLine& TieLine::getBoundaryLine1() const {
    if(!static_cast<bool>(m_boundaryLine1)) {
        throw AssertionError(stdcxx::format("boundary line 1 missing from tie line %1%", getId()));
    }
    return stdcxx::cref<BoundaryLine>(m_boundaryLine1).get();
}
BoundaryLine& TieLine::getBoundaryLine1() {
    if(!static_cast<bool>(m_boundaryLine1)) {
        throw AssertionError(stdcxx::format("boundary line 1 missing from tie line %1%", getId()));
    }
    return m_boundaryLine1.get();
}

const BoundaryLine& TieLine::getBoundaryLine2() const {
    if(!static_cast<bool>(m_boundaryLine2)) {
        throw AssertionError(stdcxx::format("boundary line 2 missing from tie line %1%", getId()));
    }
    return stdcxx::cref<BoundaryLine>(m_boundaryLine2).get();
}
BoundaryLine& TieLine::getBoundaryLine2() {
    if(!static_cast<bool>(m_boundaryLine2)) {
        throw AssertionError(stdcxx::format("boundary line 2 missing from tie line %1%", getId()));
    }
    return m_boundaryLine2.get();
}

const BoundaryLine& TieLine::getBoundaryLine(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            if(!static_cast<bool>(m_boundaryLine1)) {
                throw AssertionError(stdcxx::format("boundary line 1 missing from tie line %1%", getId()));
            }
            return m_boundaryLine1.get();
        case TwoSides::TWO:
            if(!static_cast<bool>(m_boundaryLine2)) {
                throw AssertionError(stdcxx::format("boundary line 2 missing from tie line %1%", getId()));
            }
            return m_boundaryLine2.get();
        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unknown branch side %1%", side));
    }
}
BoundaryLine& TieLine::getBoundaryLine(const TwoSides& side) {
    return const_cast<BoundaryLine&>(static_cast<const TieLine*>(this)->getBoundaryLine(side)); //NOSONAR
}

const BoundaryLine& TieLine::getBoundaryLine(const std::string& voltageLevelId) const {
    if(static_cast<bool>(m_boundaryLine1) && m_boundaryLine1.get().getTerminal().getVoltageLevel().getId() == voltageLevelId) {
        return m_boundaryLine1.get();
    } else if (static_cast<bool>(m_boundaryLine1) && m_boundaryLine2.get().getTerminal().getVoltageLevel().getId() == voltageLevelId) {
        return m_boundaryLine2.get();
    } else {
        throw AssertionError(stdcxx::format("Voltage level %1% not found on attached boundary lines", voltageLevelId));
    }
}

BoundaryLine& TieLine::getBoundaryLine(const std::string& voltageLevelId) {
    return const_cast<BoundaryLine&>(static_cast<const TieLine*>(this)->getBoundaryLine(voltageLevelId)); //NOSONAR
}

const IdentifiableType& TieLine::getType() const {
    static IdentifiableType s_type = IdentifiableType::TIE_LINE;
    return s_type;
}

const std::string& TieLine::getTypeDescription() const {
    static std::string s_typeDescription = "AC tie line";

    return s_typeDescription;
}

std::string TieLine::getPairingKey() const {
    std::string pairingKey = "";
    if(static_cast<bool>(m_boundaryLine1) && !m_boundaryLine1.get().getPairingKey().empty()) {
        pairingKey = m_boundaryLine1.get().getPairingKey();
    } else if(static_cast<bool>(m_boundaryLine2) && !m_boundaryLine2.get().getPairingKey().empty()) {
        pairingKey = m_boundaryLine2.get().getPairingKey();
    }
    return pairingKey;
}

void TieLine::remove() {
    remove(false);
}

void TieLine::remove(bool updateBoundaryLines) {
    //detach boundary lines
    if(static_cast<bool>(m_boundaryLine1)) {
        if(updateBoundaryLines) {
            updateBoundaryLine(m_boundaryLine1.get());
        }
        m_boundaryLine1.get().removeTieLine();
        m_boundaryLine1.reset();
    }
    if(static_cast<bool>(m_boundaryLine2)) {
        if(updateBoundaryLines) {
            updateBoundaryLine(m_boundaryLine2.get());
        }
        m_boundaryLine2.get().removeTieLine();
        m_boundaryLine2.reset();
    }

    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();

    // Remove this from the network
    getNetwork().remove(*this);
}

const FlowsLimitsHolder& TieLine::getOperationalLimitsHolder1() const {
    return getBoundaryLine1();
}
FlowsLimitsHolder& TieLine::getOperationalLimitsHolder1() {
    return getBoundaryLine1();
}
const FlowsLimitsHolder& TieLine::getOperationalLimitsHolder2() const {
    return getBoundaryLine2();
}
FlowsLimitsHolder& TieLine::getOperationalLimitsHolder2() {
    return getBoundaryLine2();
}

const Terminal& TieLine::getTerminal1() const {
    return getBoundaryLine1().getTerminal();
}

Terminal& TieLine::getTerminal1() {
    return getBoundaryLine1().getTerminal();
}

const Terminal& TieLine::getTerminal2() const {
    return getBoundaryLine2().getTerminal();
}

Terminal& TieLine::getTerminal2() {
    return getBoundaryLine2().getTerminal();
}

ActivePowerLimitsAdder TieLine::newActivePowerLimits1() {
    return getOrCreateSelectedOperationalLimitsGroup1().newActivePowerLimits();
}

ActivePowerLimitsAdder TieLine::newActivePowerLimits2() {
    return getOrCreateSelectedOperationalLimitsGroup2().newActivePowerLimits();
}

ApparentPowerLimitsAdder TieLine::newApparentPowerLimits1() {
    return getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits();
}

ApparentPowerLimitsAdder TieLine::newApparentPowerLimits2() {
    return getOrCreateSelectedOperationalLimitsGroup2().newApparentPowerLimits();
}

CurrentLimitsAdder TieLine::newCurrentLimits1() {
    return getOrCreateSelectedOperationalLimitsGroup1().newCurrentLimits();
}

CurrentLimitsAdder TieLine::newCurrentLimits2() {
    return getOrCreateSelectedOperationalLimitsGroup2().newCurrentLimits();
}

void TieLine::updateBoundaryLine(BoundaryLine& boundaryLine) {

    // Only update if we have values
    if(!std::isnan(boundaryLine.getBoundary().getP())) {
        boundaryLine.setP0(-boundaryLine.getBoundary().getP());
        if (static_cast<bool>(boundaryLine.getGeneration())) {
            // We do not reset regulation if we only have computed a dc load flow
            boundaryLine.getGeneration().get().setTargetP(0.0);
        }
    }
    if (!std::isnan(boundaryLine.getBoundary().getQ())){
        boundaryLine.setQ0(-boundaryLine.getBoundary().getQ());
        if (static_cast<bool>(boundaryLine.getGeneration())){
            // If q values are available a complete ac load flow has been computed, we reset regulation
            boundaryLine.getGeneration().get().setTargetQ(0.0).setVoltageRegulationOn(false).setTargetV(stdcxx::nan());
        }
    }
}

bool TieLine::connectBoundaryLines() {
    return connectBoundaryLines(SwitchPredicate::IS_NONFICTIONAL_BREAKER());
}
bool TieLine::connectBoundaryLines(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    return connectBoundaryLines(isTypeSwitchToOperate, stdcxx::optional<TwoSides>());
}
bool TieLine::connectBoundaryLines(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<TwoSides>& side) {
    return ConnectDisconnectUtil::connectAllTerminals(getTerminalsOfBoundaryLines(side), isTypeSwitchToOperate);
}

bool TieLine::disconnectBoundaryLines() {
    return disconnectBoundaryLines(SwitchPredicate::IS_NONFICTIONAL_CLOSED_BREAKER());
}
bool TieLine::disconnectBoundaryLines(const stdcxx::Predicate<Switch>& isSwitchOpenable) {
    return disconnectBoundaryLines(isSwitchOpenable, stdcxx::optional<TwoSides>());
}
bool TieLine::disconnectBoundaryLines(const stdcxx::Predicate<Switch>& isSwitchOpenable, const stdcxx::optional<TwoSides>& side) {
    return ConnectDisconnectUtil::disconnectAllTerminals(getTerminalsOfBoundaryLines(side), isSwitchOpenable);
}

std::vector<std::reference_wrapper<Terminal>> TieLine::getTerminalsOfBoundaryLines(const stdcxx::optional<TwoSides>& side) {
    std::vector<std::reference_wrapper<Terminal>> terminals;
    terminals.reserve(2);
    if(!side.has_value() || side==TwoSides::ONE) {
        terminals.push_back(std::ref(getTerminal1()));
    }
    if(!side.has_value() || side==TwoSides::TWO) {
        terminals.push_back(std::ref(getTerminal2()));
    }
    terminals.shrink_to_fit();
    return terminals;
}

}  // namespace iidm

}  // namespace powsybl
