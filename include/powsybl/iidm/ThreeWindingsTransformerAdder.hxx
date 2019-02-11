/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HXX
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HXX

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

const double& checkOptional(const Validable& validable, const stdcxx::optional<double>& value, const std::string& message);

template <typename L>
ThreeWindingsTransformerAdder::LegAdder<L>::LegAdder(ThreeWindingsTransformerAdder& parent) :
    m_parent(parent),
    m_r(stdcxx::nan()),
    m_x(stdcxx::nan()),
    m_ratedU(stdcxx::nan()) {
}

template <typename L>
ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::LegAdder<L>::add() {
    return m_parent;
}

template <typename L>
std::unique_ptr<Terminal> ThreeWindingsTransformerAdder::LegAdder<L>::checkAndGetTerminal() {
    return TerminalBuilder(m_parent.getNetwork(), *this)
               .setNode(m_node)
               .setBus(m_bus)
               .setConnectableBus(m_connectableBus)
               .build();
}

template <typename L>
VoltageLevel& ThreeWindingsTransformerAdder::LegAdder<L>::checkAndGetVoltageLevel() {
    checkNotEmpty(*this, m_voltageLevelId, "voltage level is not set");

    stdcxx::Reference<VoltageLevel> voltageLevel = m_parent.getNetwork().template find<VoltageLevel>(m_voltageLevelId);
    if (!voltageLevel) {
        throw ValidationException(*this, logging::format("voltage level '%1%' not found", m_voltageLevelId));
    }
    if (!stdcxx::areSame(voltageLevel.get().getSubstation(), m_parent.getSubstation())) {
        throw ValidationException(*this, logging::format("voltage level shall belong to the substation '%1%'", m_parent.getSubstation().getId()));
    }

    return voltageLevel.get();
}

template <typename L>
void ThreeWindingsTransformerAdder::LegAdder<L>::checkParams() const {
    checkOptional(*this, m_r, "r is not set");
    checkOptional(*this, m_x, "x is not set");
    checkOptional(*this, m_ratedU, "rated U is not set");
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::clear() {
    m_voltageLevelId.clear();
    m_node.reset();
    m_bus.clear();
    m_connectableBus.clear();
    m_r = stdcxx::nan();
    m_x = stdcxx::nan();
    m_ratedU = stdcxx::nan();

    return static_cast<L&>(*this);
}

template <typename L>
double ThreeWindingsTransformerAdder::LegAdder<L>::getR() const {
    return m_r;
}

template <typename L>
double ThreeWindingsTransformerAdder::LegAdder<L>::getRatedU() const {
    return m_ratedU;
}

template <typename L>
double ThreeWindingsTransformerAdder::LegAdder<L>::getX() const {
    return m_x;
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setBus(const std::string& bus) {
    m_bus = bus;

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setNode(unsigned long node) {
    m_node = node;

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setR(double r) {
    m_r = r;

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setRatedU(double ratedU) {
    m_ratedU = ratedU;

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setVoltageLevel(const std::string& voltageLevelId) {
    m_voltageLevelId = voltageLevelId;

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformerAdder::LegAdder<L>::setX(double x) {
    m_x = x;

    return static_cast<L&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERADDER_HXX
