/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANTARRAY_HXX
#define POWSYBL_IIDM_VARIANTARRAY_HXX

#include <powsybl/iidm/VariantArray.hpp>

#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
VariantArray<T>::VariantArray(VariantManagerHolder& variantManagerHolder, const VariantFactory& variantFactory) :
    m_variantManagerHolder(variantManagerHolder) {

    const auto& variantManager = variantManagerHolder.getVariantManager();
    m_variants.resize(variantManager.getVariantArraySize());
    for (unsigned long i : variantManager.getVariantIndices()) {
        m_variants.at(i) = variantFactory();
    }
}

template <typename T>
VariantArray<T>::VariantArray(VariantArray&& variantArray) noexcept :
    m_variantManagerHolder(variantArray.m_variantManagerHolder),
    m_variants(std::move(variantArray.m_variants)) {
}

template <typename T>
void VariantArray<T>::allocateVariantArrayElement(const std::set<unsigned long>& indexes, const VariantFactory& variantFactory) {
    for (auto index : indexes) {
        m_variants[index] = variantFactory();
    }
}

template <typename T>
std::unique_ptr<T> VariantArray<T>::copy(unsigned long index) const {
    return m_variants[index]->copy();
}

template <typename T>
void VariantArray<T>::deleteVariantArrayElement(unsigned long index) {
    m_variants[index].reset();
}

template <typename T>
void VariantArray<T>::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, const VariantFactory& variantFactory) {
    for (unsigned long i = 0; i < number; ++i) {
        m_variants.emplace_back(std::move(variantFactory()));
    }
}

template <typename T>
const T& VariantArray<T>::get() const {
    return *m_variants.at(m_variantManagerHolder.getVariantIndex());
}

template <typename T>
T& VariantArray<T>::get() {
    return *m_variants.at(m_variantManagerHolder.getVariantIndex());
}

template <typename T>
void VariantArray<T>::reduceVariantArraySize(unsigned long number) {
    m_variants.resize(m_variants.size() - number);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANTARRAY_HXX
