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
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
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

const DanglingLine& TieLine::getDanglingLine(const Branch::Side& branchSide) const {
    switch (branchSide) {
        case Branch::Side::ONE:
            if(!static_cast<bool>(m_danglingLine1)) {
                throw AssertionError(stdcxx::format("dangling line 1 missing from tie line %1%", getId()));
            }
            return m_danglingLine1.get();
        case Branch::Side::TWO:
            if(!static_cast<bool>(m_danglingLine2)) {
                throw AssertionError(stdcxx::format("dangling line 2 missing from tie line %1%", getId()));
            }
            return m_danglingLine2.get();
        default:
            throw AssertionError(stdcxx::format("Unknown branch side %1%", branchSide));
    }
}
DanglingLine& TieLine::getDanglingLine(const Branch::Side& branchSide) {
    return const_cast<DanglingLine&>(static_cast<const TieLine*>(this)->getDanglingLine(branchSide)); //NOSONAR
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

std::string TieLine::getUcteXnodeCode() const {
    std::string ucteXnodeCode = "";
    if(static_cast<bool>(m_danglingLine1) && !m_danglingLine1.get().getUcteXnodeCode().empty()) {
        ucteXnodeCode = m_danglingLine1.get().getUcteXnodeCode();
    } else if(static_cast<bool>(m_danglingLine2) && !m_danglingLine2.get().getUcteXnodeCode().empty()) {
        ucteXnodeCode = m_danglingLine2.get().getUcteXnodeCode();
    }
    return ucteXnodeCode;
}

void TieLine::remove() {
    //detach dangling lines
    if(static_cast<bool>(m_danglingLine1)) {
        m_danglingLine1.get().removeTieLine();
        m_danglingLine1.reset();
    }
    if(static_cast<bool>(m_danglingLine2)) {
        m_danglingLine2.get().removeTieLine();
        m_danglingLine2.reset();
    }

    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getSynchronousComponentsManager().invalidate();

    // Remove this from the network
    getNetwork().remove(*this);
}

stdcxx::CReference<ActivePowerLimits> TieLine::getActivePowerLimits1() const {
    return getDanglingLine1().getActivePowerLimits();
}

stdcxx::Reference<ActivePowerLimits> TieLine::getActivePowerLimits1() {
    return getDanglingLine1().getActivePowerLimits();
}

stdcxx::CReference<ActivePowerLimits> TieLine::getActivePowerLimits2() const {
    return getDanglingLine2().getActivePowerLimits();
}

stdcxx::Reference<ActivePowerLimits> TieLine::getActivePowerLimits2() {
    return getDanglingLine2().getActivePowerLimits();
}

stdcxx::CReference<ApparentPowerLimits> TieLine::getApparentPowerLimits1() const {
    return getDanglingLine1().getApparentPowerLimits();
}

stdcxx::Reference<ApparentPowerLimits> TieLine::getApparentPowerLimits1() {
    return getDanglingLine1().getApparentPowerLimits();
}

stdcxx::CReference<ApparentPowerLimits> TieLine::getApparentPowerLimits2() const {
    return getDanglingLine2().getApparentPowerLimits();
}

stdcxx::Reference<ApparentPowerLimits> TieLine::getApparentPowerLimits2() {
    return getDanglingLine2().getApparentPowerLimits();
}


stdcxx::CReference<CurrentLimits> TieLine::getCurrentLimits1() const {
    return getDanglingLine1().getCurrentLimits();
}

stdcxx::Reference<CurrentLimits> TieLine::getCurrentLimits1() {
    return getDanglingLine1().getCurrentLimits();
}

stdcxx::CReference<CurrentLimits> TieLine::getCurrentLimits2() const {
    return getDanglingLine2().getCurrentLimits();
}

stdcxx::Reference<CurrentLimits> TieLine::getCurrentLimits2() {
    return getDanglingLine2().getCurrentLimits();
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

}  // namespace iidm

}  // namespace powsybl
