/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATION_HXX
#define POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATION_HXX

#include <powsybl/iidm/extensions/cvg/VoltageRegulation.hpp>

#include <powsybl/iidm/Substation.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

template <typename T>
VoltageRegulation<T>::VoltageRegulation(T& extendable, bool voltageRegulatorOn, double targetV) :
    VoltageRegulation<T>(extendable, stdcxx::cref(extendable.getTerminal()), voltageRegulatorOn, targetV) {

}

template <typename T>
VoltageRegulation<T>::VoltageRegulation(T& extendable, const stdcxx::CReference<Terminal>& regulatingTerminal, bool voltageRegulatorOn, double targetV) :
    AbstractMultiVariantConnectableExtension(extendable) {

    setRegulatingTerminal(regulatingTerminal);

    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_voltageRegulatorOn.resize(variantArraySize, voltageRegulatorOn);
    m_targetV.resize(variantArraySize, targetV);
}

template <typename T>
void VoltageRegulation<T>::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_voltageRegulatorOn[index] = m_voltageRegulatorOn[sourceIndex];
        m_targetV[index] = m_targetV[sourceIndex];
    }
}

template <typename T>
void VoltageRegulation<T>::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && (!stdcxx::isInstanceOf<Injection>(extendable.get()) || !stdcxx::isInstanceOf<ReactiveLimitsHolder>(extendable.get()))) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% & %3% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Injection>(), stdcxx::demangle<ReactiveLimitsHolder>()));
    }
}

template <typename T>
void VoltageRegulation<T>::checkRegulatingTerminal(const stdcxx::CReference<Terminal>& regulatingTerminal, const Network& network) {
    if (regulatingTerminal && !stdcxx::areSame(regulatingTerminal.get().getVoltageLevel().getSubstation().getNetwork(), network)) {
        throw PowsyblException(logging::format("regulating terminal is not part of the same network"));
    }
}

template <typename T>
void VoltageRegulation<T>::deleteVariantArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

template <typename T>
void VoltageRegulation<T>::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() + number, m_voltageRegulatorOn[sourceIndex]);
    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
}

template <typename T>
const std::string& VoltageRegulation<T>::getName() const {
    static std::string s_name = "voltageRegulation";
    return s_name;
}

template <typename T>
const Network& VoltageRegulation<T>::getNetworkFromExtendable() const {
    return AbstractMultiVariantConnectableExtension::getExtendable<T>().get().getTerminal().getVoltageLevel().getSubstation().getNetwork();
}

template <typename T>
stdcxx::CReference<Terminal> VoltageRegulation<T>::getRegulatingTerminal() const {
    return m_regulatingTerminal;
}

template <typename T>
double VoltageRegulation<T>::getTargetV() const {
    return m_targetV[getVariantIndex()];
}

template <typename T>
const std::type_index& VoltageRegulation<T>::getType() const {
    static std::type_index s_type = typeid(VoltageRegulation<T>);
    return s_type;
}

template <typename T>
bool VoltageRegulation<T>::isVoltageRegulatorOn() const {
    return m_voltageRegulatorOn[getVariantIndex()];
}

template <typename T>
void VoltageRegulation<T>::reduceVariantArraySize(unsigned long number) {
    m_voltageRegulatorOn.resize(m_voltageRegulatorOn.size() - number);
    m_targetV.resize(m_targetV.size() - number);
}

template <typename T>
VoltageRegulation<T>& VoltageRegulation<T>::setRegulatingTerminal(const stdcxx::CReference<Terminal>& regulatingTerminal) {
    checkRegulatingTerminal(regulatingTerminal, getNetworkFromExtendable());
    m_regulatingTerminal = regulatingTerminal ? regulatingTerminal : stdcxx::cref(getExtendable<T>().get().getTerminal());
    return *this;
}

template <typename T>
VoltageRegulation<T>& VoltageRegulation<T>::setTargetV(double targetV) {
    m_targetV[getVariantIndex()] = targetV;
    return *this;
}

template <typename T>
VoltageRegulation<T>& VoltageRegulation<T>::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn[getVariantIndex()] = voltageRegulatorOn;
    return *this;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATION_HXX
