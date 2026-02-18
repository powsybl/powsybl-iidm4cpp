/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformerLeg.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace three_windings_transformer {

Leg::Leg(const ThreeSides& side, double r, double x, double g, double b, double ratedU, double ratedS) :
    m_side(checkThreeSides(*this, side)),
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_g(checkG(*this, g)),
    m_b(checkB(*this, b)),
    m_ratedU(checkRatedU(*this, ratedU, static_cast<unsigned int>(side))),
    m_ratedS(checkRatedS(*this, ratedS)),
    m_operationalLimitsGroups(stdcxx::format("limits%1%", static_cast<unsigned int>(side))) {
}

double Leg::getB() const {
    return m_b;
}

double Leg::getG() const {
    return m_g;
}

std::string Leg::getMessageHeader() const {
    return stdcxx::format("%1% '%2%': ", getTypeDescription(), m_transformer.get().getId());
}

const Network& Leg::getNetwork() const {
    return m_transformer.get().getNetwork();
}

Network& Leg::getNetwork() {
    return m_transformer.get().getNetwork();
}

const Network& Leg::getParentNetwork() const {
    return m_transformer.get().getParentNetwork();
}

Network& Leg::getParentNetwork() {
    return m_transformer.get().getParentNetwork();
}

const PhaseTapChanger& Leg::getPhaseTapChanger() const {
    if (!m_phaseTapChanger) {
        throw PowsyblException("Phase tap changer not set");
    }
    return *m_phaseTapChanger;
}

PhaseTapChanger& Leg::getPhaseTapChanger() {
    if (!m_phaseTapChanger) {
        throw PowsyblException("Phase tap changer not set");
    }
    return *m_phaseTapChanger;
}

double Leg::getR() const {
    return m_r;
}

double Leg::getRatedS() const {
    return m_ratedS;
}

double Leg::getRatedU() const {
    return m_ratedU;
}

const RatioTapChanger& Leg::getRatioTapChanger() const {
    if (!m_ratioTapChanger) {
        throw PowsyblException("Ratio tap changer not set");
    }
    return *m_ratioTapChanger;
}

RatioTapChanger& Leg::getRatioTapChanger() {
    if (!m_ratioTapChanger) {
        throw PowsyblException("Ratio tap changer not set");
    }
    return *m_ratioTapChanger;
}

unsigned long Leg::getRegulatingTapChangerCount() const {
    unsigned long count = 0;

    const Leg& leg1 = m_transformer.get().getLeg1();
    count += (leg1.hasPhaseTapChanger() && leg1.getPhaseTapChanger().isRegulating()) ? 1 : 0;
    count += (leg1.hasRatioTapChanger() && leg1.getRatioTapChanger().isRegulating()) ? 1 : 0;

    const Leg& leg2 = m_transformer.get().getLeg2();
    count += (leg2.hasPhaseTapChanger() && leg2.getPhaseTapChanger().isRegulating()) ? 1 : 0;
    count += (leg2.hasRatioTapChanger() && leg2.getRatioTapChanger().isRegulating()) ? 1 : 0;

    const Leg& leg3 = m_transformer.get().getLeg3();
    count += (leg3.hasPhaseTapChanger() && leg3.getPhaseTapChanger().isRegulating()) ? 1 : 0;
    count += (leg3.hasRatioTapChanger() && leg3.getRatioTapChanger().isRegulating()) ? 1 : 0;

    return count;
}

const Terminal& Leg::getTerminal() const {
    return m_transformer.get().getTerminal(static_cast<unsigned int>(m_side) - 1);
}

Terminal& Leg::getTerminal() {
    return m_transformer.get().getTerminal(static_cast<unsigned int>(m_side) - 1);
}

ThreeSides Leg::getSide() const {
    return m_side;
}

stdcxx::CReference<LoadingLimits> Leg::getLimits(const LimitType& type) const {
    switch (type) {
        case LimitType::CURRENT:
            return stdcxx::cref<LoadingLimits>(getCurrentLimits());

        case LimitType::ACTIVE_POWER:
            return stdcxx::cref<LoadingLimits>(getActivePowerLimits());

        case LimitType::APPARENT_POWER:
            return stdcxx::cref<LoadingLimits>(getApparentPowerLimits());

        case LimitType::VOLTAGE:
        case LimitType::VOLTAGE_ANGLE:
        default:
            throw AssertionError(stdcxx::format("Getting %1% limits is not supported.", type));
    }
}

stdcxx::Reference<LoadingLimits> Leg::getLimits(const LimitType& type) {
    return stdcxx::ref(const_cast<const Leg*>(this)->getLimits(type));
}

stdcxx::CReference<ThreeWindingsTransformer> Leg::getTransformer() const {
    return stdcxx::cref(m_transformer);
}

stdcxx::Reference<ThreeWindingsTransformer> Leg::getTransformer() {
    return m_transformer;
}

const std::string& Leg::getTypeDescription() const {
    static std::array<std::string, 3> s_typeDescriptions {{
        u8"3 windings transformer leg1",
        u8"3 windings transformer leg2",
        u8"3 windings transformer leg3"
    }};

    return s_typeDescriptions[static_cast<unsigned int>(m_side) - 1];
}

double Leg::getX() const {
    return m_x;
}

bool Leg::hasPhaseTapChanger() const {
    return static_cast<bool>(m_phaseTapChanger);
}

bool Leg::hasRatioTapChanger() const {
    return static_cast<bool>(m_ratioTapChanger);
}

Leg& Leg::setB(double b) {
    m_b = checkB(*this, b);
    return *this;
}

Leg& Leg::setG(double g) {
    m_g = checkG(*this, g);
    return *this;
}

Leg& Leg::setR(double r) {
    m_r = checkR(*this, r);
    return *this;
}

Leg& Leg::setRatedS(double ratedS) {
    m_ratedS = checkRatedS(*this, ratedS);
    return *this;
}

Leg& Leg::setRatedU(double ratedU) {
    m_ratedU = checkRatedU(*this, ratedU);
    return *this;
}

PhaseTapChangerAdder Leg::newPhaseTapChanger() {
    return PhaseTapChangerAdder(*this);
}

RatioTapChangerAdder Leg::newRatioTapChanger() {
    return RatioTapChangerAdder(*this);
}

Leg& Leg::setX(double x) {
    m_x = checkX(*this, x);
    return *this;
}

void Leg::setPhaseTapChanger(std::unique_ptr<PhaseTapChanger>&& phaseTapChanger) {
    m_phaseTapChanger = std::move(phaseTapChanger);
}

void Leg::setRatioTapChanger(std::unique_ptr<RatioTapChanger>&& ratioTapChanger) {
    m_ratioTapChanger = std::move(ratioTapChanger);
}

Leg& Leg::setTransformer(ThreeWindingsTransformer& transformer) {
    m_transformer = transformer;
    m_operationalLimitsGroups.setIdentifiable(transformer);
    return *this;
}

std::string Leg::toString() const {
    return stdcxx::format("%1% leg%2%", m_transformer.get().getId(), static_cast<unsigned int>(m_side));
}

stdcxx::const_range<OperationalLimitsGroup> Leg::getOperationalLimitsGroups() const {
    return m_operationalLimitsGroups.getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> Leg::getOperationalLimitsGroups() {
    return m_operationalLimitsGroups.getOperationalLimitsGroups();
}
const stdcxx::optional<std::string>& Leg::getSelectedOperationalLimitsGroupId() const {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroupId();
}
stdcxx::CReference<OperationalLimitsGroup> Leg::getOperationalLimitsGroup(const std::string& id) const {
    return m_operationalLimitsGroups.getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> Leg::getOperationalLimitsGroup(const std::string& id) {
    return m_operationalLimitsGroups.getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> Leg::getSelectedOperationalLimitsGroup() const {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> Leg::getSelectedOperationalLimitsGroup() {
    return m_operationalLimitsGroups.getSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& Leg::newOperationalLimitsGroup(const std::string& id) {
    return m_operationalLimitsGroups.newOperationalLimitsGroup(id);
}
void Leg::setSelectedOperationalLimitsGroup(const std::string& id) {
    m_operationalLimitsGroups.setSelectedOperationalLimitsGroup(id);
}
void Leg::removeOperationalLimitsGroup(const std::string& id) {
    m_operationalLimitsGroups.removeOperationalLimitsGroup(id);
}
void Leg::cancelSelectedOperationalLimitsGroup() {
    m_operationalLimitsGroups.cancelSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& Leg::getOrCreateSelectedOperationalLimitsGroup() {
    return m_operationalLimitsGroups.getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& Leg::getOrCreateSelectedOperationalLimitsGroup(const std::string& id) {
    return FlowsLimitsHolder::getOrCreateSelectedOperationalLimitsGroup(id);
}

ActivePowerLimitsAdder Leg::newActivePowerLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits();
}
ApparentPowerLimitsAdder Leg::newApparentPowerLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits();
}
CurrentLimitsAdder Leg::newCurrentLimits() {
    return getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits();
}

}  // namespace three_windings_transformer

std::ostream& operator<<(std::ostream& stream, const three_windings_transformer::Leg& leg) {
    stream << leg.toString();

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
