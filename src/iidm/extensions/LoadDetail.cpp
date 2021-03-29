/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/LoadDetail.hpp>

#include <cmath>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

LoadDetail::LoadDetail(Load& load, double fixedActivePower, double fixedReactivePower, double variableActivePower, double variableReactivePower) :
    AbstractMultiVariantIdentifiableExtension(load) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_fixedActivePower.resize(variantArraySize, checkPower(fixedActivePower, "Invalid fixedActivePower"));
    m_fixedReactivePower.resize(variantArraySize, checkPower(fixedReactivePower, "Invalid fixedReactivePower"));
    m_variableActivePower.resize(variantArraySize, checkPower(variableActivePower, "Invalid variableActivePower"));
    m_variableReactivePower.resize(variantArraySize, checkPower(variableReactivePower, "Invalid variableReactivePower"));
}

void LoadDetail::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_fixedActivePower[index] = m_fixedActivePower[sourceIndex];
        m_fixedReactivePower[index] = m_fixedReactivePower[sourceIndex];
        m_variableActivePower[index] = m_variableActivePower[sourceIndex];
        m_variableReactivePower[index] = m_variableReactivePower[sourceIndex];
    }
}

void LoadDetail::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Load>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Load>()));
    }
}

double LoadDetail::checkPower(double value, const std::string& message) {
    if (std::isnan(value)) {
        throw PowsyblException(message);
    }
    return value;
}

void LoadDetail::deleteVariantArrayElement(unsigned long /*index*/) {
    // Does nothing
}

void LoadDetail::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_fixedActivePower.resize(m_fixedActivePower.size() + number, m_fixedActivePower[sourceIndex]);
    m_fixedReactivePower.resize(m_fixedReactivePower.size() + number, m_fixedReactivePower[sourceIndex]);
    m_variableActivePower.resize(m_variableActivePower.size() + number, m_variableActivePower[sourceIndex]);
    m_variableReactivePower.resize(m_variableReactivePower.size() + number, m_variableReactivePower[sourceIndex]);
}

double LoadDetail::getFixedActivePower() const {
    return m_fixedActivePower[getVariantIndex()];
}

double LoadDetail::getFixedReactivePower() const {
    return m_fixedReactivePower[getVariantIndex()];
}

const std::string& LoadDetail::getName() const {
    static std::string s_name = "detail";
    return s_name;
}

const std::type_index& LoadDetail::getType() const {
    static std::type_index s_type = typeid(LoadDetail);
    return s_type;
}

double LoadDetail::getVariableActivePower() const {
    return m_variableActivePower[getVariantIndex()];
}

double LoadDetail::getVariableReactivePower() const {
    return m_variableReactivePower[getVariantIndex()];
}

void LoadDetail::reduceVariantArraySize(unsigned long number) {
    m_fixedActivePower.resize(m_fixedActivePower.size() - number);
    m_fixedReactivePower.resize(m_fixedReactivePower.size() - number);
    m_variableActivePower.resize(m_variableActivePower.size() - number);
    m_variableReactivePower.resize(m_variableReactivePower.size() - number);
}

LoadDetail& LoadDetail::setFixedActivePower(double fixedActivePower) {
    m_fixedActivePower[getVariantIndex()] = checkPower(fixedActivePower, "Invalid fixedActivePower");
    return *this;
}

LoadDetail& LoadDetail::setFixedReactivePower(double fixedReactivePower) {
    m_fixedReactivePower[getVariantIndex()] = checkPower(fixedReactivePower, "Invalid fixedReactivePower");
    return *this;
}

LoadDetail& LoadDetail::setVariableActivePower(double variableActivePower) {
    m_variableActivePower[getVariantIndex()] = checkPower(variableActivePower, "Invalid variableActivePower");
    return *this;
}

LoadDetail& LoadDetail::setVariableReactivePower(double variableReactivePower) {
    m_variableReactivePower[getVariantIndex()] = checkPower(variableReactivePower, "Invalid variableReactivePower");
    return *this;
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
