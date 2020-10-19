/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANT_HPP
#define POWSYBL_IIDM_VARIANT_HPP

#include <memory>

namespace powsybl {

namespace iidm {

class VariantManagerHolder;

template <typename T>
class Variant {
public:
    virtual ~Variant() noexcept = default;

    virtual std::unique_ptr<T> copy() const = 0;

    virtual void setVariantManagerHolder(VariantManagerHolder& variantManagerHolder);
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Variant.hxx>

#endif  // POWSYBL_IIDM_VARIANT_HPP
