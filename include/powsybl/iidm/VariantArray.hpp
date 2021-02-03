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

namespace powsybl {

namespace iidm {

template <typename Owner, typename V, typename = typename std::enable_if<std::is_base_of<Variant<Owner, V>, V>::value>>
class VariantArray {
public:
    using VariantFactory = std::function<std::unique_ptr<V>()>;

public:
    VariantArray(Owner& owner, const VariantFactory& variantFactory);

    VariantArray(const VariantArray&) = delete;

    VariantArray(VariantArray&&) noexcept = delete;

    VariantArray(Owner& owner, VariantArray&& variantArray);

    ~VariantArray() noexcept = default;

    VariantArray& operator=(const VariantArray&) = delete;

    VariantArray& operator=(VariantArray&&) noexcept = delete;

    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, const VariantFactory& variantFactory);

    std::unique_ptr<V> copy(unsigned long index) const;

    void deleteVariantArrayElement(unsigned long index);

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, const VariantFactory& variantFactory);

    const V& get() const;

    V& get();

    void reduceVariantArraySize(unsigned long number);

private:
    Owner& m_owner;

    std::vector<std::unique_ptr<V> > m_variants;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/VariantArray.hxx>

#endif  // POWSYBL_IIDM_VARIANTARRAY_HPP
