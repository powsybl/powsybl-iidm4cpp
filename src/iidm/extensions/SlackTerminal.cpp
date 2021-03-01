/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/SlackTerminal.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/SlackTerminalAdder.hpp>
#include <powsybl/iidm/util/TerminalFinder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

SlackTerminal::SlackTerminal(VoltageLevel& voltageLevel, Terminal& terminal) :
    AbstractMultiVariantIdentifiableExtension(voltageLevel) {
    m_terminals.resize(getVariantManagerHolder().getVariantManager().getVariantArraySize());
    setTerminal(stdcxx::ref(terminal));
}

void SlackTerminal::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    const auto& terminalSource = m_terminals[sourceIndex];
    for (auto index : indexes) {
        m_terminals[index] = terminalSource;
    }
}

void SlackTerminal::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<VoltageLevel>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<VoltageLevel>()));
    }
}

void SlackTerminal::attach(Bus& bus) {
    VoltageLevel& vl = bus.getVoltageLevel();
    stdcxx::Reference<Terminal> terminal = TerminalFinder::find(bus.getConnectedTerminals());
    if (!terminal) {
        throw PowsyblException(stdcxx::format("Unable to find a terminal in the bus %1%", bus.getId()));
    }
    reset(vl, terminal);
}

void SlackTerminal::deleteVariantArrayElement(unsigned long index) {
    m_terminals[index].reset();
}

void SlackTerminal::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_terminals.resize(m_terminals.size() + number, m_terminals[sourceIndex]);
}

const std::string& SlackTerminal::getName() const {
    static std::string s_name = "slackTerminal";
    return s_name;
}

stdcxx::CReference<Terminal> SlackTerminal::getTerminal() const {
    return stdcxx::cref(m_terminals[getVariantIndex()]);
}

stdcxx::Reference<Terminal> SlackTerminal::getTerminal() {
    return m_terminals[getVariantIndex()];
}

const std::type_index& SlackTerminal::getType() const {
    static std::type_index s_type = typeid(SlackTerminal);
    return s_type;
}

bool SlackTerminal::isEmpty() const {
    auto it = std::find_if(m_terminals.begin(), m_terminals.end(), [](const stdcxx::Reference<Terminal>& terminal) {
        return static_cast<bool>(terminal);
    });
    return it == m_terminals.end();
}

void SlackTerminal::reduceVariantArraySize(unsigned long number) {
    m_terminals.resize(number); // remove elements from the top to avoid moves inside the array
}

void SlackTerminal::reset(Network& network) {
    for (auto& vl : network.getVoltageLevels()) {
        reset(vl, stdcxx::ref<Terminal>());
    }
}

void SlackTerminal::reset(VoltageLevel& voltageLevel, const stdcxx::Reference<Terminal>& terminal) {
    auto st = voltageLevel.findExtension<SlackTerminal>();
    if (!st && terminal) {
        voltageLevel.newExtension<SlackTerminalAdder>().withTerminal(terminal.get()).add();
    } else if (st) {
        st.get().setTerminal(terminal, true);
    }
}

SlackTerminal& SlackTerminal::setTerminal(const stdcxx::CReference<Terminal>& terminal) {
    if (terminal && !stdcxx::areSame(terminal.get().getVoltageLevel(), getExtendable().get())) {
        throw PowsyblException(stdcxx::format("Terminal given is not in the right VoltageLevel (%1% instead of %2%)", terminal.get().getVoltageLevel().getId(), getExtendable<VoltageLevel>().get().getId()));
    }
    m_terminals[getVariantIndex()] = stdcxx::ref(terminal);
    return *this;
}

SlackTerminal& SlackTerminal::setTerminal(const stdcxx::Reference<Terminal>& terminal) {
    return setTerminal(stdcxx::cref(terminal));
}

SlackTerminal& SlackTerminal::setTerminal(const stdcxx::CReference<Terminal>& terminal, bool cleanIfEmpty) {
    setTerminal(terminal);
    if (cleanIfEmpty && isEmpty()) {
        getExtendable().get().removeExtension<SlackTerminal>();
    }
    return *this;
}

SlackTerminal& SlackTerminal::setTerminal(const stdcxx::Reference<Terminal>& terminal, bool cleanIfEmpty) {
    return setTerminal(stdcxx::cref(terminal), cleanIfEmpty);
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
