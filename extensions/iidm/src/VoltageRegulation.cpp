/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/VoltageRegulation.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

VoltageRegulation::VoltageRegulation(Battery& battery, bool voltageRegulatorOn, double targetV) :
    VoltageRegulation(battery, stdcxx::ref(battery.getTerminal()), voltageRegulatorOn, targetV) {
}

VoltageRegulation::VoltageRegulation(Battery& battery, const stdcxx::Reference<Terminal>& regulatingTerminal, bool voltageRegulatorOn, double targetV) :
    AbstractMultiVariantIdentifiableExtension(battery) {

    setRegulatingTerminal(regulatingTerminal);

    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_voltageRegulatorOn.resize(variantArraySize, voltageRegulatorOn);
    m_targetV.resize(variantArraySize, targetV);
}

void VoltageRegulation::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_targetV[index] = m_targetV[sourceIndex];
    }
}

void VoltageRegulation::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>()));
    }
}

void VoltageRegulation::checkTerminalInNetwork(const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network) {
    if (regulatingTerminal && !stdcxx::areSame(regulatingTerminal.get().getVoltageLevel().getNetwork(), network)) {
        throw PowsyblException(stdcxx::format("regulating terminal is not part of the same network"));
    }
}

void VoltageRegulation::deleteVariantArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

void VoltageRegulation::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
}

const std::string& VoltageRegulation::getName() const {
    static std::string s_name = "voltageRegulation";
    return s_name;
}

const Network& VoltageRegulation::getNetworkFromExtendable() const {
    return AbstractMultiVariantIdentifiableExtension::getExtendable<Battery>().get().getTerminal().getVoltageLevel().getNetwork();
}

stdcxx::CReference<Terminal> VoltageRegulation::getRegulatingTerminal() const {
    return stdcxx::cref(m_regulatingTerminal);
}

double VoltageRegulation::getTargetV() const {
    return m_targetV[getVariantIndex()];
}

const std::type_index& VoltageRegulation::getType() const {
    static std::type_index s_type = typeid(VoltageRegulation);
    return s_type;
}

bool VoltageRegulation::isVoltageRegulatorOn() const {
    return m_voltageRegulatorOn[getVariantIndex()];
}

void VoltageRegulation::reduceVariantArraySize(unsigned long number) {
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_targetV.resize(m_targetV.size() - number);
}

VoltageRegulation& VoltageRegulation::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    checkTerminalInNetwork(regulatingTerminal, getNetworkFromExtendable());
    m_regulatingTerminal = regulatingTerminal ? regulatingTerminal : stdcxx::ref(getExtendable<Battery>().get().getTerminal());
    return *this;
}

VoltageRegulation& VoltageRegulation::setTargetV(double targetV) {
    m_targetV[getVariantIndex()] = targetV;
    return *this;
}

VoltageRegulation& VoltageRegulation::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn[getVariantIndex()] = voltageRegulatorOn;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
