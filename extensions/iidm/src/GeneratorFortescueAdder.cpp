/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorFortescueAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorFortescue.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorFortescueAdder::GeneratorFortescueAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> GeneratorFortescueAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return stdcxx::make_unique<GeneratorFortescue>(dynamic_cast<Generator&>(extendable),
                                                        m_grounded, m_rz, m_xz, m_rn, m_xn, 
                                                        m_groundingR, m_groundingX);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

GeneratorFortescueAdder& GeneratorFortescueAdder::withGrounded(bool grounded) {
    m_grounded = grounded;
    return *this;
}
GeneratorFortescueAdder& GeneratorFortescueAdder::withRz(double rz) {
    m_rz = rz;
    return *this;
}
GeneratorFortescueAdder& GeneratorFortescueAdder::withXz(double xz) {
    m_xz = xz;
    return *this;
}
GeneratorFortescueAdder& GeneratorFortescueAdder::withRn(double rn) {
    m_rn = rn;
    return *this;
}
GeneratorFortescueAdder& GeneratorFortescueAdder::withXn(double xn) {
    m_xn = xn;
    return *this;
}
GeneratorFortescueAdder& GeneratorFortescueAdder::withGroundingR(double groundingR) {
    m_groundingR = groundingR;
    return *this;
}
GeneratorFortescueAdder& GeneratorFortescueAdder::withGroundingX(double groundingX) {
    m_groundingX = groundingX;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
