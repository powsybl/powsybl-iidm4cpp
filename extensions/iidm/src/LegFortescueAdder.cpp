/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LegFortescueAdder.hpp>

#include <powsybl/iidm/extensions/iidm/LegFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescueAdder.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LegFortescueAdder::LegFortescueAdder(ThreeWindingsTransformerFortescueAdder& parent, unsigned long legNumber, WindingConnectionType connectionType) :
    m_parent(parent),
    m_legNumber(legNumber),
    m_connectionType(connectionType){
}

ThreeWindingsTransformerFortescueAdder& LegFortescueAdder::add(){
    m_parent.setLegAdder(*this, m_legNumber);
    return m_parent;
}

LegFortescue LegFortescueAdder::build() const{
    return LegFortescue(m_rz, m_xz, m_freeFluxes, m_connectionType, m_groundingR, m_groundingX);
}

LegFortescueAdder& LegFortescueAdder::withRz(double rz) {
    m_rz = rz;
    return *this;
}
LegFortescueAdder& LegFortescueAdder::withXz(double xz) {
    m_xz = xz;
    return *this;
}
LegFortescueAdder& LegFortescueAdder::withFreeFluxes(bool freeFluxes) {
    m_freeFluxes = freeFluxes;
    return *this;
}
LegFortescueAdder& LegFortescueAdder::withGroundingR(double groundingR) {
    m_groundingR = groundingR;
    return *this;
}
LegFortescueAdder& LegFortescueAdder::withGroundingX(double groundingX) {
    m_groundingX = groundingX;
    return *this;
}
LegFortescueAdder& LegFortescueAdder::withConnectionType(WindingConnectionType connectionType) {
    m_connectionType = connectionType;
    return *this;
}


}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

