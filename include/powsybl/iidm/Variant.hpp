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

template <typename Owner, typename V>
class Variant {
public:
    explicit Variant(Owner& owner);

    Variant(const Variant&) = default;

    Variant(Variant&&) noexcept = delete;

    virtual ~Variant() noexcept = default;

    Variant& operator=(const Variant&) = delete;

    Variant& operator=(Variant&&) noexcept = delete;

    virtual std::unique_ptr<V> copy() const = 0;

protected:
    Owner& m_owner;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Variant.hxx>

#endif  // POWSYBL_IIDM_VARIANT_HPP
