/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuitAdder.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuit.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorShortCircuitAdder::GeneratorShortCircuitAdder(Extendable& extendable) :
    AbstractShortCircuitAdder(extendable) {
}

std::unique_ptr<Extension> GeneratorShortCircuitAdder::createExtension(Extendable& extendable) const {
    GeneratorShortCircuit::checkTransX(m_directTransX);
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return std::unique_ptr<GeneratorShortCircuit>(new GeneratorShortCircuit(dynamic_cast<Generator&>(extendable), m_directSubtransX, m_directTransX, m_stepUpTransformerX));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
