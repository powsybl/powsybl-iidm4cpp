/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANTARRAY_HPP
#define POWSYBL_IIDM_VARIANTARRAY_HPP

#include <functional>
#include <memory>
#include <set>
#include <vector>

#include <powsybl/iidm/Ref.hpp>
#include <powsybl/iidm/Variant.hpp>

namespace powsybl {

namespace iidm {

class Network;
class VariantManagerHolder;

template <typename T>
class VariantArray {
public:
    using VariantFactory = std::function<std::unique_ptr<T>()>;

public:
    VariantArray(VariantManagerHolder& variantManagerHolder, const VariantFactory& variantFactory);

    VariantArray(VariantArray&& variantArray) noexcept;

    ~VariantArray() noexcept = default;

    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, const VariantFactory& variantFactory);

    std::unique_ptr<T> copy(unsigned long index) const;

    void deleteVariantArrayElement(unsigned long index);

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, const VariantFactory& variantFactory);

    const T& get() const;

    T& get();

    void reduceVariantArraySize(unsigned long number);

    void setVariantManagerHolder(VariantManagerHolder& variantManagerHolder);

private:
    Ref<VariantManagerHolder> m_variantManagerHolder;

    std::vector<std::unique_ptr<T> > m_variants;
};

}  // namespace iidm

}  // namespace powsybl

#include "VariantArray.hxx"

#endif  // POWSYBL_IIDM_VARIANTARRAY_HPP
