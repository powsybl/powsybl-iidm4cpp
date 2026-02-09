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
    m_danglingLine1.reset();
    m_danglingLine2.reset();
}

void TieLine::attachDanglingLines(DanglingLine& dl1, DanglingLine& dl2) {
    m_danglingLine1 = attach(dl1);
    m_danglingLine2 = attach(dl2);
}
DanglingLine& TieLine::attach(DanglingLine& dl) {
    dl.setTieLine(*this);
    return dl;
}

const Network& TieLine::getNetwork() const {
    return m_network.get();
}

Network& TieLine::getNetwork() {
    return m_network.get();
}

const Network& TieLine::getParentNetwork() const {
    if(m_danglingLine1 && m_danglingLine2) {
        const Network& subNetwork1 = m_danglingLine1.get().getParentNetwork();
        const Network& subNetwork2 = m_danglingLine2.get().getParentNetwork();
        if(stdcxx::areSame(subNetwork1, subNetwork2)) {
            return subNetwork1;
        }
    } else if(m_danglingLine1) {
        return m_danglingLine1.get().getParentNetwork();
    } else if(m_danglingLine2) {
        return m_danglingLine2.get().getParentNetwork();
    }
    return getNetwork();
}

Network& TieLine::getParentNetwork() {
    return const_cast<Network&>(static_cast<const TieLine*>(this)->getParentNetwork());
}

double TieLine::getB1() const {
    return TieLineUtil::getB1(getDanglingLine1(), getDanglingLine2());
}

double TieLine::getB2() const {
    return TieLineUtil::getB2(getDanglingLine1(), getDanglingLine2());
}

double TieLine::getG1() const {
    return TieLineUtil::getG1(getDanglingLine1(), getDanglingLine2());
}

double TieLine::getG2() const {
    return TieLineUtil::getG2(getDanglingLine1(), getDanglingLine2());
}

double TieLine::getR() const {
    return TieLineUtil::getR(getDanglingLine1(), getDanglingLine2());
}

double TieLine::getX() const {
    return TieLineUtil::getX(getDanglingLine1(), getDanglingLine2());
}


const DanglingLine& TieLine::getDanglingLine1() const {
    if(!static_cast<bool>(m_danglingLine1)) {
        throw AssertionError(stdcxx::format("dangling line 1 missing from tie line %1%", getId()));
    }
    return stdcxx::cref<DanglingLine>(m_danglingLine1).get();
}
DanglingLine& TieLine::getDanglingLine1() {
    if(!static_cast<bool>(m_danglingLine1)) {
        throw AssertionError(stdcxx::format("dangling line 1 missing from tie line %1%", getId()));
    }
    return m_danglingLine1.get();
}

const DanglingLine& TieLine::getDanglingLine2() const {
    if(!static_cast<bool>(m_danglingLine2)) {
        throw AssertionError(stdcxx::format("dangling line 2 missing from tie line %1%", getId()));
    }
    return stdcxx::cref<DanglingLine>(m_danglingLine2).get();
}
DanglingLine& TieLine::getDanglingLine2() {
    if(!static_cast<bool>(m_danglingLine2)) {
        throw AssertionError(stdcxx::format("dangling line 2 missing from tie line %1%", getId()));
    }
    return m_danglingLine2.get();
}

const DanglingLine& TieLine::getDanglingLine(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            if(!static_cast<bool>(m_danglingLine1)) {
                throw AssertionError(stdcxx::format("dangling line 1 missing from tie line %1%", getId()));
            }
            return m_danglingLine1.get();
        case TwoSides::TWO:
            if(!static_cast<bool>(m_danglingLine2)) {
                throw AssertionError(stdcxx::format("dangling line 2 missing from tie line %1%", getId()));
            }
            return m_danglingLine2.get();
        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unknown branch side %1%", side));
    }
}
DanglingLine& TieLine::getDanglingLine(const TwoSides& side) {
    return const_cast<DanglingLine&>(static_cast<const TieLine*>(this)->getDanglingLine(side)); //NOSONAR
}

const DanglingLine& TieLine::getDanglingLine(const std::string& voltageLevelId) const {
    if(static_cast<bool>(m_danglingLine1) && m_danglingLine1.get().getTerminal().getVoltageLevel().getId() == voltageLevelId) {
        return m_danglingLine1.get();
    } else if (static_cast<bool>(m_danglingLine1) && m_danglingLine2.get().getTerminal().getVoltageLevel().getId() == voltageLevelId) {
        return m_danglingLine2.get();
    } else {
        throw AssertionError(stdcxx::format("Voltage level %1% not found on attached dangling lines", voltageLevelId));
    }
}

DanglingLine& TieLine::getDanglingLine(const std::string& voltageLevelId) {
    return const_cast<DanglingLine&>(static_cast<const TieLine*>(this)->getDanglingLine(voltageLevelId)); //NOSONAR
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
    if(static_cast<bool>(m_danglingLine1) && !m_danglingLine1.get().getPairingKey().empty()) {
        pairingKey = m_danglingLine1.get().getPairingKey();
    } else if(static_cast<bool>(m_danglingLine2) && !m_danglingLine2.get().getPairingKey().empty()) {
        pairingKey = m_danglingLine2.get().getPairingKey();
    }
    return pairingKey;
}

void TieLine::remove() {
    remove(false);
}

void TieLine::remove(bool updateDanglingLines) {
    //detach dangling lines
    if(static_cast<bool>(m_danglingLine1)) {
        if(updateDanglingLines) {
            updateDanglingLine(m_danglingLine1.get());
        }
        m_danglingLine1.get().removeTieLine();
        m_danglingLine1.reset();
    }
    if(static_cast<bool>(m_danglingLine2)) {
        if(updateDanglingLines) {
            updateDanglingLine(m_danglingLine2.get());
        }
        m_danglingLine2.get().removeTieLine();
        m_danglingLine2.reset();
    }

    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();

    // Remove this from the network
    getNetwork().remove(*this);
}

stdcxx::const_range<OperationalLimitsGroup> TieLine::getOperationalLimitsGroups1() const {
    return getDanglingLine1().getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> TieLine::getOperationalLimitsGroups1() {
    return getDanglingLine1().getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& TieLine::getSelectedOperationalLimitsGroupId1() const {
    return getDanglingLine1().getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> TieLine::getOperationalLimitsGroup1(const std::string& id) const {
    return getDanglingLine1().getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> TieLine::getOperationalLimitsGroup1(const std::string& id) {
    return getDanglingLine1().getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> TieLine::getSelectedOperationalLimitsGroup1() const {
    return getDanglingLine1().getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> TieLine::getSelectedOperationalLimitsGroup1() {
    return getDanglingLine1().getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> TieLine::newOperationalLimitsGroup1(const std::string& id) {
    return getDanglingLine1().newOperationalLimitsGroup(id);
}
void TieLine::setSelectedOperationalLimitsGroup1(const std::string& id) {
    getDanglingLine1().setSelectedOperationalLimitsGroup(id);
}
void TieLine::removeOperationalLimitsGroup1(const std::string& id) {
    getDanglingLine1().removeOperationalLimitsGroup(id);
}
void TieLine::cancelSelectedOperationalLimitsGroup1() {
    getDanglingLine1().cancelSelectedOperationalLimitsGroup();
}

stdcxx::const_range<OperationalLimitsGroup> TieLine::getOperationalLimitsGroups2() const {
    return getDanglingLine2().getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> TieLine::getOperationalLimitsGroups2() {
    return getDanglingLine2().getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& TieLine::getSelectedOperationalLimitsGroupId2() const {
    return getDanglingLine2().getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> TieLine::getOperationalLimitsGroup2(const std::string& id) const {
    return getDanglingLine2().getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> TieLine::getOperationalLimitsGroup2(const std::string& id) {
    return getDanglingLine2().getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> TieLine::getSelectedOperationalLimitsGroup2() const {
    return getDanglingLine2().getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> TieLine::getSelectedOperationalLimitsGroup2() {
    return getDanglingLine2().getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> TieLine::newOperationalLimitsGroup2(const std::string& id) {
    return getDanglingLine2().newOperationalLimitsGroup(id);
}
void TieLine::setSelectedOperationalLimitsGroup2(const std::string& id) {
    getDanglingLine2().setSelectedOperationalLimitsGroup(id);
}
void TieLine::removeOperationalLimitsGroup2(const std::string& id) {
    getDanglingLine2().removeOperationalLimitsGroup(id);
}
void TieLine::cancelSelectedOperationalLimitsGroup2() {
    getDanglingLine2().cancelSelectedOperationalLimitsGroup();
}

const Terminal& TieLine::getTerminal1() const {
    return getDanglingLine1().getTerminal();
}

Terminal& TieLine::getTerminal1() {
    return getDanglingLine1().getTerminal();
}

const Terminal& TieLine::getTerminal2() const {
    return getDanglingLine2().getTerminal();
}

Terminal& TieLine::getTerminal2() {
    return getDanglingLine2().getTerminal();
}

ActivePowerLimitsAdder TieLine::newActivePowerLimits1() {
    return getDanglingLine1().newActivePowerLimits();
}

ActivePowerLimitsAdder TieLine::newActivePowerLimits2() {
    return getDanglingLine2().newActivePowerLimits();
}

ApparentPowerLimitsAdder TieLine::newApparentPowerLimits1() {
    return getDanglingLine1().newApparentPowerLimits();
}

ApparentPowerLimitsAdder TieLine::newApparentPowerLimits2() {
    return getDanglingLine2().newApparentPowerLimits();
}

CurrentLimitsAdder TieLine::newCurrentLimits1() {
    return getDanglingLine1().newCurrentLimits();
}

CurrentLimitsAdder TieLine::newCurrentLimits2() {
    return getDanglingLine2().newCurrentLimits();
}

void TieLine::updateDanglingLine(DanglingLine& danglingLine) {

    // Only update if we have values
    if(!std::isnan(danglingLine.getBoundary().getP())) {
        danglingLine.setP0(-danglingLine.getBoundary().getP());
        if (static_cast<bool>(danglingLine.getGeneration())) {
            // We do not reset regulation if we only have computed a dc load flow
            danglingLine.getGeneration().get().setTargetP(0.0);
        }
    }
    if (!std::isnan(danglingLine.getBoundary().getQ())){
        danglingLine.setQ0(-danglingLine.getBoundary().getQ());
        if (static_cast<bool>(danglingLine.getGeneration())){
            // If q values are available a complete ac load flow has been computed, we reset regulation
            danglingLine.getGeneration().get().setTargetQ(0.0).setVoltageRegulationOn(false).setTargetV(stdcxx::nan());
        }
    }
}

bool TieLine::connectDanglingLines() {
    return connectDanglingLines(SwitchPredicate::IS_NONFICTIONAL_BREAKER());
}
bool TieLine::connectDanglingLines(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    return connectDanglingLines(isTypeSwitchToOperate, stdcxx::optional<TwoSides>());
}
bool TieLine::connectDanglingLines(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<TwoSides>& side) {
    return ConnectDisconnectUtil::connectAllTerminals(getTerminalsOfDanglingLines(side), isTypeSwitchToOperate);
}

bool TieLine::disconnectDanglingLines() {
    return disconnectDanglingLines(SwitchPredicate::IS_CLOSED_BREAKER());
}
bool TieLine::disconnectDanglingLines(const stdcxx::Predicate<Switch>& isSwitchOpenable) {
    return disconnectDanglingLines(isSwitchOpenable, stdcxx::optional<TwoSides>());
}
bool TieLine::disconnectDanglingLines(const stdcxx::Predicate<Switch>& isSwitchOpenable, const stdcxx::optional<TwoSides>& side) {
    return ConnectDisconnectUtil::disconnectAllTerminals(getTerminalsOfDanglingLines(side), isSwitchOpenable);
}

std::vector<std::reference_wrapper<Terminal>> TieLine::getTerminalsOfDanglingLines(const stdcxx::optional<TwoSides>& side) {
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
