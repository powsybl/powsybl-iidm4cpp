/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferenceTerminalsAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/ReferenceTerminals.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferenceTerminalsAdder::ReferenceTerminalsAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ReferenceTerminalsAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Network>(extendable)) {
        return std::unique_ptr<ReferenceTerminals>(new ReferenceTerminals(dynamic_cast<Network&>(extendable), m_terminals));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Network>()));
}

ReferenceTerminalsAdder& ReferenceTerminalsAdder::withTerminals(const std::vector<stdcxx::Reference<Terminal>>& terminals){
    m_terminals = terminals;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
