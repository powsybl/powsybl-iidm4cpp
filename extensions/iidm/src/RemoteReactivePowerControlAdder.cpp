/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControlAdder.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControl.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

RemoteReactivePowerControlAdder::RemoteReactivePowerControlAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> RemoteReactivePowerControlAdder::createExtension(Extendable& extendable) {
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return std::unique_ptr<RemoteReactivePowerControl>(new RemoteReactivePowerControl(dynamic_cast<Generator&>(extendable), m_targetQ, m_regulatingTerminal.get(), m_enabled));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

RemoteReactivePowerControlAdder& RemoteReactivePowerControlAdder::withEnabled(bool enabled) {
    m_enabled = enabled;
    return *this;
}

RemoteReactivePowerControlAdder& RemoteReactivePowerControlAdder::withRegulatingTerminal(Terminal& regulatingTerminal) {
    m_regulatingTerminal = regulatingTerminal;
    return *this;
}

RemoteReactivePowerControlAdder& RemoteReactivePowerControlAdder::withTargetQ(double targetQ) {
    m_targetQ = targetQ;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
