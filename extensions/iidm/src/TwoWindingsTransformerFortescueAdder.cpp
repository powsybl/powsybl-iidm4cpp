/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescueAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescue.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerFortescueAdder::TwoWindingsTransformerFortescueAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> TwoWindingsTransformerFortescueAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable)) {
        return stdcxx::make_unique<TwoWindingsTransformerFortescue>(dynamic_cast<TwoWindingsTransformer&>(extendable), m_rz, m_xz, m_freeFluxes,
                                    m_connectionType1, m_connectionType2, m_groundingR1, m_groundingX1, m_groundingR2, m_groundingX2);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<TwoWindingsTransformer>()));
}

TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withRz(double rz) {
    m_rz = rz;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withXz(double xz) {
    m_xz = xz;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withFreeFluxes(bool freeFluxes) {
    m_freeFluxes = freeFluxes;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withConnectionType1(WindingConnectionType connectionType1) {
    m_connectionType1 = connectionType1;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withConnectionType2(WindingConnectionType connectionType2) {
    m_connectionType2 = connectionType2;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withGroundingR1(double groundingR1) {
    m_groundingR1 = groundingR1;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withGroundingX1(double groundingX1) {
    m_groundingX1 = groundingX1;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withGroundingR2(double groundingR2) {
    m_groundingR2 = groundingR2;
    return *this;
}
TwoWindingsTransformerFortescueAdder& TwoWindingsTransformerFortescueAdder::withGroundingX2(double groundingX2) {
    m_groundingX2 = groundingX2;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
