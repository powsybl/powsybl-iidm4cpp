/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferenceTerminals.hpp>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/iidm/ReferenceTerminalsAdder.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferenceTerminals::ReferenceTerminals(Network& network, const std::vector<stdcxx::Reference<Terminal>>& terminals) :
    AbstractMultiVariantIdentifiableExtension(network) {
        m_referenceTerminals.resize(getVariantManagerHolder().getVariantManager().getVariantArraySize(), std::vector<stdcxx::Reference<Terminal>>());
        setReferenceTerminals(terminals);
}

const std::string& ReferenceTerminals::getName() const {
    static std::string s_name = "referenceTerminals";
    return s_name;
}

const std::type_index& ReferenceTerminals::getType() const {
    static std::type_index s_type = typeid(ReferenceTerminals);
    return s_type;
}

void ReferenceTerminals::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_referenceTerminals[index] = m_referenceTerminals[sourceIndex];
    }
}
void ReferenceTerminals::deleteVariantArrayElement(unsigned long index) {
    m_referenceTerminals[index].clear();
}
void ReferenceTerminals::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_referenceTerminals.resize(m_referenceTerminals.size() + number, m_referenceTerminals[sourceIndex]);
}
void ReferenceTerminals::reduceVariantArraySize(unsigned long number) {
    m_referenceTerminals.resize(m_referenceTerminals.size() - number);
}

ReferenceTerminals& ReferenceTerminals::setReferenceTerminals(const std::vector<stdcxx::Reference<Terminal>>& terminals) {
    for(auto& terminal : terminals) {
        checkTerminalInNetwork(terminal, getExtendable<Network>().get());
    }
    reset();
    for(auto& terminal : terminals) {
        addReferenceTerminal(terminal);
    }
    return *this;
}

ReferenceTerminals& ReferenceTerminals::addReferenceTerminal(Terminal& terminalRef) {
    checkTerminalInNetwork(terminalRef, getExtendable<Network>().get());

    // If the provided terminal already in the list, nothing to do
    for (auto& existingRef : m_referenceTerminals[getVariantIndex()]) {
        if(static_cast<bool>(existingRef) && stdcxx::areSame(existingRef.get(), terminalRef)) {
            return *this;
        }
    }

    m_referenceTerminals[getVariantIndex()].push_back(stdcxx::ref(terminalRef));
    return *this;
}

std::vector<stdcxx::Reference<Terminal>>& ReferenceTerminals::getReferenceTerminals() {
    return m_referenceTerminals[getVariantIndex()];
}

stdcxx::const_range<Terminal> ReferenceTerminals::getReferenceTerminals() const {
    const auto& mapper = stdcxx::map<stdcxx::Reference<Terminal>, Terminal>;
    return m_referenceTerminals[getVariantIndex()] | boost::adaptors::transformed(mapper);
}

ReferenceTerminals& ReferenceTerminals::reset() {
    m_referenceTerminals[getVariantIndex()].clear();
    return *this;
}

void ReferenceTerminals::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Network>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Network>()));
    }
}

std::vector<stdcxx::Reference<Terminal>> ReferenceTerminals::getTerminals(Network& network) {
    std::vector<stdcxx::Reference<Terminal>> terminals;

    stdcxx::Reference<ReferenceTerminals> refExtension = network.findExtension<ReferenceTerminals>();
    if(static_cast<bool>(refExtension)) {
        terminals = refExtension.get().getReferenceTerminals();
    }
    return terminals;
}

void ReferenceTerminals::addTerminal(Terminal& terminal) {
    Network& network = terminal.getVoltageLevel().getParentNetwork();
    stdcxx::Reference<ReferenceTerminals> refExtension = network.findExtension<ReferenceTerminals>();
    if(!static_cast<bool>(refExtension)) {
        network.newExtension<ReferenceTerminalsAdder>()
                .withTerminals(std::vector<stdcxx::Reference<Terminal>>())
                .add();
    }
    ReferenceTerminals& ext = network.getExtension<ReferenceTerminals>();
    ext.addReferenceTerminal(terminal);
}

void ReferenceTerminals::reset(Network& network) {
    stdcxx::Reference<ReferenceTerminals> refExtension = network.findExtension<ReferenceTerminals>();
    if(static_cast<bool>(refExtension)) {
        refExtension.get().reset();
    }
}

void ReferenceTerminals::checkTerminalInNetwork(const Terminal& terminal, const Network& network) {
    if(!network.contains(terminal.getVoltageLevel())){
        throw PowsyblException(stdcxx::format("Terminal given is not in the right Network (%1% instead of %2%)",
             terminal.getVoltageLevel().getParentNetwork().getId(), network.getId()));
    }
}


}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
