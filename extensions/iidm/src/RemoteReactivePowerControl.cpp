/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControl.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

RemoteReactivePowerControl::RemoteReactivePowerControl(Generator& generator, double targetQ, Terminal& terminal, bool enabled) :
    AbstractMultiVariantIdentifiableExtension(generator),
    m_regulatingTerminal(terminal) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_targetQ.resize(variantArraySize, targetQ);
    m_enabled.resize(variantArraySize, enabled);
    if (!stdcxx::areSame(terminal.getVoltageLevel().getParentNetwork(), generator.getParentNetwork())) {
        throw PowsyblException( stdcxx::format("Regulating terminal is not in the right Network (%1% instead of %2%)",
            terminal.getVoltageLevel().getParentNetwork().getId(), generator.getParentNetwork().getId()) );
    }
    terminal.registerReferrer(*this);
}

void RemoteReactivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

const std::string& RemoteReactivePowerControl::getName() const {
    static std::string s_name = "generatorRemoteReactivePowerControl";
    return s_name;
}

const Terminal& RemoteReactivePowerControl::getRegulatingTerminal() const {
    return m_regulatingTerminal.get();
}

Terminal& RemoteReactivePowerControl::getRegulatingTerminal() {
    return m_regulatingTerminal.get();
}

double RemoteReactivePowerControl::getTargetQ() const {
    return m_targetQ[getVariantIndex()];
}

const std::type_index& RemoteReactivePowerControl::getType() const {
    static std::type_index s_type = typeid(RemoteReactivePowerControl);
    return s_type;
}

bool RemoteReactivePowerControl::isEnabled() const {
    return m_enabled[getVariantIndex()];
}

RemoteReactivePowerControl& RemoteReactivePowerControl::setTargetQ(double targetQ) {
    m_targetQ[getVariantIndex()] = checkTargetQ(targetQ);
    return *this;
}
RemoteReactivePowerControl& RemoteReactivePowerControl::setEnabled(bool enabled) {
    m_enabled[getVariantIndex()] = enabled;
    return *this;
}
RemoteReactivePowerControl& RemoteReactivePowerControl::setRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal) {
    checkRegulatingTerminal(terminal, getExtendable<Generator>().get().getNetwork());
    if(terminal && !stdcxx::areSame(m_regulatingTerminal.get(), terminal.get())) {
        getRegulatingTerminal().unregisterReferrer(*this);
        m_regulatingTerminal = terminal.get();
        terminal.get().registerReferrer(*this);
    }
    return *this;
}

double RemoteReactivePowerControl::checkTargetQ(double targetQ) {
    if(std::isnan(targetQ)) {
        throw PowsyblException(stdcxx::format("RemoteReactivePowerControl's reactive power target must be set"));
    }
    return targetQ;
}

void RemoteReactivePowerControl::checkRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal, const Network& network) {
    if(!terminal) {
        throw PowsyblException("RemoteReactivePowerControl's regulating terminal must be set");
    }
    if (terminal && !stdcxx::areSame(terminal.get().getVoltageLevel().getNetwork(), network)) {
        throw PowsyblException("Regulating terminal is not part of the same network");
    }
}

void RemoteReactivePowerControl::onReferencedRemoval(Terminal& /*removedReference*/) {
    // we can't set regulating terminal to null to stay consistent and cannot also fallback to a local terminal so we just remove the extension
    logging::Logger& logger = logging::LoggerFactory::getLogger<RemoteReactivePowerControl>();
    logger.warn(stdcxx::format("Remove 'RemoteReactivePowerControl' extension of generator '%1%', because its regulating terminal has been removed",
                        getExtendable<Generator>().get().getId()));
    getExtendable<Generator>().get().removeExtension<RemoteReactivePowerControl>();
}

void RemoteReactivePowerControl::onReferencedReplacement(Terminal& /*oldReference*/, Terminal& newReference) {
    checkRegulatingTerminal(stdcxx::ref(newReference), getExtendable<Generator>().get().getNetwork());
    m_regulatingTerminal = newReference;
    newReference.registerReferrer(*this);
}

void RemoteReactivePowerControl::cleanup() {
    getRegulatingTerminal().unregisterReferrer(*this);
}

void RemoteReactivePowerControl::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_targetQ[index] = m_targetQ[sourceIndex];
        m_enabled[index] = m_enabled[sourceIndex];
    }
}

void RemoteReactivePowerControl::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void RemoteReactivePowerControl::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_targetQ.resize(m_targetQ.size() + number, m_targetQ[sourceIndex]);
    m_enabled.resize(m_enabled.size() + number, m_enabled[sourceIndex]);
}

void RemoteReactivePowerControl::reduceVariantArraySize(unsigned long number) {
    m_targetQ.resize(m_targetQ.size() - number);
    m_enabled.resize(m_enabled.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
