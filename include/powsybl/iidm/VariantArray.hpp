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

#include <powsybl/iidm/Variant.hpp>
#include <powsybl/iidm/VariantManagerSupplier.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename = typename std::enable_if<std::is_base_of<Variant<T>, T>::value>::type>
class VariantArray {
public:
    using VariantFactory = std::function<std::unique_ptr<T>()>;

public:
    VariantArray(const VariantManagerSupplier& supplier, const VariantFactory& variantFactory);

    VariantArray(const VariantArray& variantArray) = delete;

    VariantArray(VariantArray&& variantArray) noexcept = delete;

    ~VariantArray() noexcept = default;

    VariantArray& operator=(const VariantArray& variantArray) = delete;

    VariantArray& operator=(VariantArray&& variantArray) noexcept = delete;

    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, const VariantFactory& variantFactory);

    std::unique_ptr<T> copy(unsigned long index) const;

    void deleteVariantArrayElement(unsigned long index);

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, const VariantFactory& variantFactory);

    const T& get() const;

    T& get();

    void reduceVariantArraySize(unsigned long number);

private:
    VariantManagerSupplier m_supplier;

    std::vector<std::unique_ptr<T> > m_variants;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/VariantArray.hxx>

#endif  // POWSYBL_IIDM_VARIANTARRAY_HPP
