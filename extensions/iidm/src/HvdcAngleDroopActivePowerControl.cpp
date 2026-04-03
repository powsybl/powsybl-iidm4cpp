/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControl.hpp>

#include <cmath>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

HvdcAngleDroopActivePowerControl::HvdcAngleDroopActivePowerControl(HvdcLine& hvdcLine, double p0, double droop, bool enabled) :
    AbstractMultiVariantIdentifiableExtension(hvdcLine) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_p0.resize(variantArraySize, checkP0(p0, hvdcLine));
    m_droop.resize(variantArraySize, checkDroop(droop, hvdcLine));
    m_enabled.resize(variantArraySize, enabled);
}

void HvdcAngleDroopActivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<HvdcLine>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<HvdcLine>()));
    }
}

double HvdcAngleDroopActivePowerControl::checkDroop(double droop, const HvdcLine& line) {
    if (std::isnan(droop)) {
        throw ValidationException(line, stdcxx::format("droop value (%1%) is invalid", droop));
    }
    return droop;
}

double HvdcAngleDroopActivePowerControl::checkP0(double p0, const HvdcLine& line) {
    if (std::isnan(p0)) {
        throw ValidationException(line, stdcxx::format("p0 value (%1%) is invalid", p0));
    }
    return p0;
}

double HvdcAngleDroopActivePowerControl::getDroop() const {
    return m_droop[getVariantIndex()];
}

const std::string& HvdcAngleDroopActivePowerControl::getName() const {
    static std::string s_name = "hvdcAngleDroopActivePowerControl";
    return s_name;
}

double HvdcAngleDroopActivePowerControl::getP0() const {
    return m_p0[getVariantIndex()];
}

const std::type_index& HvdcAngleDroopActivePowerControl::getType() const {
    static std::type_index s_type = typeid(HvdcAngleDroopActivePowerControl);
    return s_type;
}

bool HvdcAngleDroopActivePowerControl::isEnabled() const {
    return m_enabled[getVariantIndex()];
}

HvdcAngleDroopActivePowerControl& HvdcAngleDroopActivePowerControl::setDroop(double droop) {
    const auto& hvdcLine = getExtendable<HvdcLine>().get();
    m_droop[getVariantIndex()] = checkDroop(droop, hvdcLine);
    return *this;
}

HvdcAngleDroopActivePowerControl& HvdcAngleDroopActivePowerControl::setEnabled(bool enabled) {
    m_enabled[getVariantIndex()] = enabled;
    return *this;
}

HvdcAngleDroopActivePowerControl& HvdcAngleDroopActivePowerControl::setP0(double p0) {
    const auto& hvdcLine = getExtendable<HvdcLine>().get();
    m_p0[getVariantIndex()] = checkP0(p0, hvdcLine);
    return *this;
}

void HvdcAngleDroopActivePowerControl::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_p0[index] = m_p0[sourceIndex];
        m_droop[index] = m_droop[sourceIndex];
        m_enabled[index] = m_enabled[sourceIndex];
    }
}

void HvdcAngleDroopActivePowerControl::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void HvdcAngleDroopActivePowerControl::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_p0.resize(m_p0.size() + number, m_p0[sourceIndex]);
    m_droop.resize(m_droop.size() + number, m_droop[sourceIndex]);
    m_enabled.resize(m_enabled.size() + number, m_enabled[sourceIndex]);
}

void HvdcAngleDroopActivePowerControl::reduceVariantArraySize(unsigned long number) {
    m_p0.resize(m_p0.size() - number);
    m_droop.resize(m_droop.size() - number);
    m_enabled.resize(m_enabled.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
