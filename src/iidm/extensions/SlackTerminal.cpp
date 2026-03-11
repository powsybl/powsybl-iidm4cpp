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
        unregisterReferencedTerminalIfNeeded(index);
        //no need to register terminalSource since it's already referenced.
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
    unregisterReferencedTerminalIfNeeded(index);
    m_terminals[index].reset();
}

void SlackTerminal::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    //No need to register terminal, since it's already referenced at sourceIndex
    m_terminals.resize(m_terminals.size() + number, m_terminals[sourceIndex]);
}

const std::string& SlackTerminal::getName() const {
    static std::string s_name = "slackTerminal";
    return s_name;
}

stdcxx::CReference<Terminal> SlackTerminal::getTerminal() const {
    return static_cast<bool>(m_terminals[getVariantIndex()]) ? stdcxx::cref(m_terminals[getVariantIndex()]) : stdcxx::CReference<Terminal>();
}

stdcxx::Reference<Terminal> SlackTerminal::getTerminal() {
    return static_cast<bool>(m_terminals[getVariantIndex()]) ? m_terminals[getVariantIndex()] : stdcxx::Reference<Terminal>();
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
    // remove elements from the top to avoid moves inside the array
    // and one by one to unregister reference correctly
    for(unsigned long index = 0; index < number ; index ++) {
        unregisterReferencedTerminalIfNeeded(m_terminals.size() - 1);
        m_terminals.resize(m_terminals.size() - 1);
    }
}

void SlackTerminal::reset(Network& network) {
    /* With current implementation of subnetworks and limitation at 1 level of subnetwork,
     * getVoltageLevels() on the root network will get the voltagelevels from subnetworks as well.
     * So there is no need to iterate through each subnetwork for now.
     */
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

SlackTerminal& SlackTerminal::setTerminal(const stdcxx::Reference<Terminal>& terminal) {
    if (terminal && !stdcxx::areSame(terminal.get().getVoltageLevel(), getExtendable().get())) {
        throw PowsyblException(stdcxx::format("Terminal given is not in the right VoltageLevel (%1% instead of %2%)", terminal.get().getVoltageLevel().getId(), getExtendable<VoltageLevel>().get().getId()));
    }

    unregisterReferencedTerminalIfNeeded(getVariantIndex());
    if(static_cast<bool>(terminal)) {
        registerReferencedTerminalIfNeeded(terminal.get());
    }
    m_terminals[getVariantIndex()] = terminal;
    return *this;
}

SlackTerminal& SlackTerminal::setTerminal(const stdcxx::Reference<Terminal>& terminal, bool cleanIfEmpty) {
    setTerminal(terminal);
    if (cleanIfEmpty && isEmpty()) {
        getExtendable().get().removeExtension<SlackTerminal>();
    }
    return *this;
}

void SlackTerminal::cleanup() {
    //Might try to unregister several times the same terminal if referenced in several variants. Not an issue, unregisterReferrer will just do nothing is *this is not registered
    for (auto& terminal : m_terminals) {
        if(static_cast<bool>(terminal)) {
            terminal.get().unregisterReferrer(*this);
        }
    }
}

void SlackTerminal::onReferencedRemoval(Terminal& removedReference) {
    for(auto& terminal : m_terminals) {
        if(static_cast<bool>(terminal) && stdcxx::areSame(terminal.get(), removedReference)) {
            terminal = stdcxx::Reference<Terminal>();
        }
    }
    if (isEmpty()) { //remove extension if there is no terminal left
        getExtendable().get().removeExtension<SlackTerminal>();
    }
}

void SlackTerminal::onReferencedReplacement(Terminal& oldReference, Terminal& newReference) {
    if (!stdcxx::areSame(newReference.getVoltageLevel(), getExtendable().get())) {
        throw PowsyblException(stdcxx::format("Terminal given is not in the right VoltageLevel (%1% instead of %2%)", newReference.getVoltageLevel().getId(), getExtendable<VoltageLevel>().get().getId()));
    }
    
    bool bRegisterOnlyOnce = true;
    
    for(auto& terminal : m_terminals) {
        if(static_cast<bool>(terminal) && stdcxx::areSame(terminal.get(), oldReference)) {
            if(bRegisterOnlyOnce) { //Only register once for all the variants
                bRegisterOnlyOnce = false;
                registerReferencedTerminalIfNeeded(newReference);
            }
            terminal = newReference;
        }
    }
}

void SlackTerminal::unregisterReferencedTerminalIfNeeded(unsigned long variantIndex) {
    auto currentVariantTerminal = m_terminals[variantIndex];
    if(!currentVariantTerminal) {
        return;
    }

    unsigned int count = 0; //Count of variants on which this terminal is referenced
    for(auto& terminal : m_terminals) {
        if (static_cast<bool>(terminal) && stdcxx::areSame(terminal.get(), currentVariantTerminal.get())) {
            count++;
        }
    }

    if(count == 1) { //current Terminal is referenced only in the current variant
        currentVariantTerminal.get().unregisterReferrer(*this);
    }
}

void SlackTerminal::registerReferencedTerminalIfNeeded(Terminal& terminal) {
    //Register the given terminal only if not already present in the list
    auto it = std::find_if(m_terminals.begin(), m_terminals.end(), [&terminal](const stdcxx::Reference<Terminal>& terminalRef){
        return (static_cast<bool>(terminalRef) && stdcxx::areSame(terminalRef.get(), terminal));
    });
    if(it != m_terminals.end()) {
        //already referenced
        return;
    }

    //given temrinal not found : register it
    terminal.registerReferrer(*this);
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
