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
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

template <typename Owner, typename V, typename Dummy>
VariantArray<Owner, V, Dummy>::VariantArray(Owner& owner, const VariantFactory& variantFactory) :
    m_owner(owner) {

    const auto& variantManager = owner.getNetwork().getVariantManager();
    m_variants.resize(variantManager.getVariantArraySize());
    for (unsigned long i : variantManager.getVariantIndices()) {
        m_variants.at(i) = variantFactory();
    }
}

template <typename Owner, typename V, typename Dummy>
VariantArray<Owner, V, Dummy>::VariantArray(Owner& owner, VariantArray&& variantArray) :
    m_owner(owner) {

    for (auto& variant : variantArray.m_variants) {
        m_variants.emplace_back(stdcxx::make_unique<V>(owner, std::move(*variant)));
    }
}

template <typename Owner, typename V, typename Dummy>
void VariantArray<Owner, V, Dummy>::allocateVariantArrayElement(const std::set<unsigned long>& indexes, const VariantFactory& variantFactory) {
    for (auto index : indexes) {
        m_variants[index] = variantFactory();
    }
}

template <typename Owner, typename V, typename Dummy>
std::unique_ptr<V> VariantArray<Owner, V, Dummy>::copy(unsigned long index) const {
    return m_variants[index]->copy();
}

template <typename Owner, typename V, typename Dummy>
void VariantArray<Owner, V, Dummy>::deleteVariantArrayElement(unsigned long index) {
    m_variants[index].reset();
}

template <typename Owner, typename V, typename Dummy>
void VariantArray<Owner, V, Dummy>::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, const VariantFactory& variantFactory) {
    for (unsigned long i = 0; i < number; ++i) {
        m_variants.emplace_back(std::move(variantFactory()));
    }
}

template <typename Owner, typename V, typename Dummy>
const V& VariantArray<Owner, V, Dummy>::get() const {
    const auto& variantManager = m_owner.getNetwork().getVariantManager();
    return *m_variants.at(variantManager.getVariantIndex());
}

template <typename Owner, typename V, typename Dummy>
V& VariantArray<Owner, V, Dummy>::get() {
    const auto& variantManager = m_owner.getNetwork().getVariantManager();
    return *m_variants.at(variantManager.getVariantIndex());
}

template <typename Owner, typename V, typename Dummy>
void VariantArray<Owner, V, Dummy>::reduceVariantArraySize(unsigned long number) {
    m_variants.resize(m_variants.size() - number);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANTARRAY_HXX
