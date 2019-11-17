/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/StateOfCharge.hpp>

#include <powsybl/iidm/VariantManager.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

StateOfCharge::StateOfCharge(Battery& battery, double storageCapacity, double current) :
    StateOfCharge(battery, 0.0, 100.0, storageCapacity, current) {
}

StateOfCharge::StateOfCharge(Battery& battery, double min, double max, double storageCapacity, double current) :
    AbstractMultiVariantConnectableExtension(battery),
    m_min(checkMin(min)),
    m_max(checkMax(max)),
    m_storageCapacity(checkStorageCapacity(storageCapacity)) {

    // Check the values (min <= current <= max)
    checkBound(min, max);
    double currentValidated = checkCurrent(current, m_min, m_max);

    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_current.resize(variantArraySize, currentValidated);
}

void StateOfCharge::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (auto index : indexes) {
        m_current[index] = m_current[sourceIndex];
    }
}

void StateOfCharge::assertExtendable(const stdcxx::Reference<powsybl::iidm::Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>()));
    }
}

void StateOfCharge::checkBound(double min, double max) {
    if (min > max) {
        throw PowsyblException(logging::format("min value cannot be greater than max"));
    }
}

double StateOfCharge::checkCurrent(double current, double min, double max) {
    if (current < min || current > max) {
        throw PowsyblException(logging::format("current value is not in the range [%1%, %2%]", min, max));
    }

    return current;
}

double StateOfCharge::checkMax(double max) {
    if (max > 100.0) {
        throw PowsyblException(logging::format("max value cannot be greater than 100"));
    }

    return max;
}

double StateOfCharge::checkMin(double min) {
    if (min < 0) {
        throw PowsyblException(logging::format("min value cannot be negative"));
    }

    return min;
}

double StateOfCharge::checkStorageCapacity(double storageCapacity) {
    if (storageCapacity < 0) {
        throw PowsyblException(logging::format("storageCapacity value cannot be negative"));
    }

    return storageCapacity;
}

void StateOfCharge::deleteVariantArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

void StateOfCharge::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_current.resize(m_current.size() + number, m_current[sourceIndex]);
}

double StateOfCharge::getCurrent() const {
    return m_current[getVariantIndex()];
}

double StateOfCharge::getCurrentEnergy() const {
    return m_current[getVariantIndex()] * m_storageCapacity / 100.0;
}

double StateOfCharge::getMax() const {
    return m_max;
}

double StateOfCharge::getMaxEnergy() const {
    return m_max * m_storageCapacity / 100.0;
}

double StateOfCharge::getMin() const {
    return m_min;
}

double StateOfCharge::getMinEnergy() const {
    return m_min * m_storageCapacity / 100.0;
}

const std::string& StateOfCharge::getName() const {
    static std::string s_name = "stateOfCharge";
    return s_name;
}

double StateOfCharge::getStorageCapacity() const {
    return m_storageCapacity;
}

const std::type_index& StateOfCharge::getType() const {
    static std::type_index s_type = typeid(StateOfCharge);
    return s_type;
}

void StateOfCharge::reduceVariantArraySize(unsigned long number) {
    m_current.resize(m_current.size() - number);
}

StateOfCharge& StateOfCharge::setCurrent(double current) {
    m_current[getVariantIndex()] = checkCurrent(current, m_min, m_max);
    return *this;
}

StateOfCharge& StateOfCharge::setMax(double max) {
    checkBound(m_min, max);
    m_max = checkMax(max);
    return *this;
}

StateOfCharge& StateOfCharge::setMin(double min) {
    checkBound(min, m_max);
    m_min = checkMin(min);
    return *this;
}

StateOfCharge& StateOfCharge::setStorageCapacity(double storageCapacity) {
    m_storageCapacity = checkStorageCapacity(storageCapacity);
    return *this;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
