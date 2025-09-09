/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LegFortescue.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LegFortescue::LegFortescue(double rz, double xz, bool freeFluxes, WindingConnectionType connectionType, double groundingR, double groundingX) :
    m_rz(rz),
    m_xz(xz),
    m_freeFluxes(freeFluxes),
    m_connectionType(connectionType),
    m_groundingR(groundingR),
    m_groundingX(groundingX) {
}

bool LegFortescue::isFreeFluxes() const {
    return m_freeFluxes;
}
double LegFortescue::getRz() const {
    return m_rz;
}
double LegFortescue::getXz() const {
    return m_xz;
}
double LegFortescue::getGroundingR() const {
    return m_groundingR;
}
double LegFortescue::getGroundingX() const {
    return m_groundingX;
}
WindingConnectionType LegFortescue::getConnectionType() const {
    return m_connectionType;
}

LegFortescue& LegFortescue::setRz(double rz) {
    m_rz = rz;
    return *this;
}
LegFortescue& LegFortescue::setXz(double xz) {
    m_xz = xz;
    return *this;
}
LegFortescue& LegFortescue::setGroundingR(double groundingR) {
    m_groundingR = groundingR;
    return *this;
}
LegFortescue& LegFortescue::setGroundingX(double groundingX) {
    m_groundingX = groundingX;
    return *this;
}
LegFortescue& LegFortescue::setConnectionType(WindingConnectionType connectionType) {
    m_connectionType = connectionType;
    return *this;
}
LegFortescue& LegFortescue::setFreeFluxes(bool freeFluxes) {
    m_freeFluxes = freeFluxes;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

