/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuitAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuit.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

IdentifiableShortCircuitAdder::IdentifiableShortCircuitAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> IdentifiableShortCircuitAdder::createExtension(Extendable& extendable) const {
    if (std::isnan(m_ipMax)) {
        throw PowsyblException("Undefined ipMax");
    }
    if (stdcxx::isInstanceOf<Identifiable>(extendable)) {
        return std::unique_ptr<IdentifiableShortCircuit>(new IdentifiableShortCircuit(dynamic_cast<Identifiable&>(extendable), m_ipMin, m_ipMax));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Identifiable>()));
}

IdentifiableShortCircuitAdder& IdentifiableShortCircuitAdder::withIpMax(double ipMax) {
    m_ipMax = ipMax;
    return *this;
}

IdentifiableShortCircuitAdder& IdentifiableShortCircuitAdder::withIpMin(double ipMin) {
    m_ipMin = ipMin;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
