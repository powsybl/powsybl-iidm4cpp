/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformerLeg.hpp>

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace three_windings_transformer {

Leg::Leg(unsigned long legNumber, double r, double x, double g, double b, double ratedU, double ratedS) :
    FlowsLimitsHolder(stdcxx::format("limits%1%", m_legNumber)),
    m_legNumber(legNumber),
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_g(checkG(*this, g)),
    m_b(checkB(*this, b)),
    m_ratedU(checkRatedU(*this, ratedU, legNumber)),
    m_ratedS(checkRatedS(*this, ratedS)) {
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
    return m_transformer.get().getTerminal(m_legNumber - 1);
}

Terminal& Leg::getTerminal() {
    return m_transformer.get().getTerminal(m_legNumber - 1);
}

const std::string& Leg::getTypeDescription() const {
    static std::array<std::string, 3> s_typeDescriptions {{
        u8"3 windings transformer leg1",
        u8"3 windings transformer leg2",
        u8"3 windings transformer leg3"
    }};

    return s_typeDescriptions[m_legNumber - 1];
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
    setIdentifiable(transformer);
    return *this;
}

std::string Leg::toString() const {
    return stdcxx::format("%1% leg%2%", m_transformer.get().getId(), m_legNumber);
}

}  // namespace three_windings_transformer

std::ostream& operator<<(std::ostream& stream, const three_windings_transformer::Leg& leg) {
    stream << leg.toString();

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
