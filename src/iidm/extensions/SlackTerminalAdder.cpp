/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/SlackTerminalAdder.hpp>

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/SlackTerminal.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

SlackTerminalAdder::SlackTerminalAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> SlackTerminalAdder::createExtension(Extendable& extendable) {
    if (!m_terminal) {
        throw PowsyblException("Terminal needs to be set to create a SlackTerminal extension");
    }
    if (stdcxx::isInstanceOf<VoltageLevel>(extendable)) {
        const auto& voltageLevel = dynamic_cast<const VoltageLevel&>(extendable);
        if (!stdcxx::areSame(m_terminal.get().getVoltageLevel(), extendable)) {
            throw PowsyblException(stdcxx::format("Terminal given is not in the right VoltageLevel (%1% instead of %2%)", m_terminal.get().getVoltageLevel().getId(), voltageLevel.getId()));
        }
        return stdcxx::make_unique<SlackTerminal>(dynamic_cast<VoltageLevel&>(extendable), m_terminal.get());
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<VoltageLevel>()));
}

SlackTerminalAdder& SlackTerminalAdder::withTerminal(Terminal& terminal) {
    m_terminal = stdcxx::ref(terminal);
    return *this;
}

SlackTerminalAdder& SlackTerminalAdder::withTerminal(const Terminal& terminal) {
    m_terminal = stdcxx::ref(terminal);
    return *this;
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
