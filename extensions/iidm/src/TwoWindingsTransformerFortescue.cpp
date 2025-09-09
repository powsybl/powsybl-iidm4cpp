/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescue.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerFortescue::TwoWindingsTransformerFortescue(TwoWindingsTransformer& twt, double rz, double xz, bool freeFluxes,
                                    WindingConnectionType connectionType1, WindingConnectionType connectionType2,
                                    double groundingR1, double groundingX1, double groundingR2, double groundingX2 ) :
    Extension(twt),
    m_rz(rz),
    m_xz(xz),
    m_freeFluxes(freeFluxes),
    m_connectionType1(connectionType1),
    m_connectionType2(connectionType2),
    m_groundingR1(groundingR1),
    m_groundingX1(groundingX1),
    m_groundingR2(groundingR2),
    m_groundingX2(groundingX2) {
}

void TwoWindingsTransformerFortescue::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<TwoWindingsTransformer>()));
    }
}

const std::string& TwoWindingsTransformerFortescue::getName() const {
    static std::string s_name = "twoWindingsTransformerFortescue";
    return s_name;
}

const std::type_index& TwoWindingsTransformerFortescue::getType() const {
    static std::type_index s_type = typeid(TwoWindingsTransformerFortescue);
    return s_type;
}

bool TwoWindingsTransformerFortescue::isFreeFluxes() const {
    return m_freeFluxes;
}
double TwoWindingsTransformerFortescue::getRz() const {
    return m_rz;
}
double TwoWindingsTransformerFortescue::getXz() const {
    return m_xz;
}
double TwoWindingsTransformerFortescue::getGroundingR1() const {
    return m_groundingR1;
}
double TwoWindingsTransformerFortescue::getGroundingX1() const {
    return m_groundingX1;
}
double TwoWindingsTransformerFortescue::getGroundingR2() const {
    return m_groundingR2;
}
double TwoWindingsTransformerFortescue::getGroundingX2() const {
    return m_groundingX2;
}
WindingConnectionType TwoWindingsTransformerFortescue::getConnectionType1() const {
    return m_connectionType1;
}
WindingConnectionType TwoWindingsTransformerFortescue::getConnectionType2() const {
    return m_connectionType2;
}

TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setFreeFluxes(bool freeFluxes) {
    m_freeFluxes = freeFluxes;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setRz(double rz) {
    m_rz = rz;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setXz(double xz) {
    m_xz = xz;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setGroundingR1(double groundingR) {
    m_groundingR1 = groundingR;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setGroundingX1(double groundingX) {
    m_groundingX1 = groundingX;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setGroundingR2(double groundingR) {
    m_groundingR2 = groundingR;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setGroundingX2(double groundingX) {
    m_groundingX2 = groundingX;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setConnectionType1(WindingConnectionType connectionType) {
    m_connectionType1 = connectionType;
    return *this;
}
TwoWindingsTransformerFortescue& TwoWindingsTransformerFortescue::setConnectionType2(WindingConnectionType connectionType) {
    m_connectionType2 = connectionType;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

