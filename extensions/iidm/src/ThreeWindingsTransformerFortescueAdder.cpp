/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescueAdder.hpp>

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescue.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerFortescueAdder::ThreeWindingsTransformerFortescueAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ThreeWindingsTransformerFortescueAdder::createExtension(Extendable& extendable) const {

    if (!m_LegAdder1) {
        throw PowsyblException(stdcxx::format("leg 1 is not defined"));
    }
    if (!m_LegAdder2) {
        throw PowsyblException(stdcxx::format("leg 2 is not defined"));
    }
    if (!m_LegAdder3) {
        throw PowsyblException(stdcxx::format("leg 3 is not defined"));
    }
    LegFortescue leg1 = m_LegAdder1->build();
    LegFortescue leg2 = m_LegAdder2->build();
    LegFortescue leg3 = m_LegAdder3->build();

    if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable)) {
        return stdcxx::make_unique<ThreeWindingsTransformerFortescue>(dynamic_cast<ThreeWindingsTransformer&>(extendable), std::move(leg1), std::move(leg2), std::move(leg3));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<ThreeWindingsTransformer>()));
}

LegFortescueAdder ThreeWindingsTransformerFortescueAdder::newLeg1() {
    return LegFortescueAdder(*this, 1, DEFAULT_LEG1_CONNECTION_TYPE);
}
LegFortescueAdder ThreeWindingsTransformerFortescueAdder::newLeg2() {
    return LegFortescueAdder(*this, 2, DEFAULT_LEG2_CONNECTION_TYPE);
}
LegFortescueAdder ThreeWindingsTransformerFortescueAdder::newLeg3() {
    return LegFortescueAdder(*this, 3, DEFAULT_LEG3_CONNECTION_TYPE);
}

void ThreeWindingsTransformerFortescueAdder::setLegAdder(const LegFortescueAdder& legAdder, unsigned long legNumber){
    switch (legNumber) {
        case 1:
            m_LegAdder1.emplace(legAdder);
            break;
        case 2:
            m_LegAdder2.emplace(legAdder);
            break;
        case 3:
            m_LegAdder3.emplace(legAdder);
            break;
        default:
            throw PowsyblException(stdcxx::format("Unexpected leg number"));
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
