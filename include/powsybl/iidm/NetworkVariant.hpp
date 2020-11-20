/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKVARIANT_HPP
#define POWSYBL_IIDM_NETWORKVARIANT_HPP

#include <powsybl/iidm/Variant.hpp>
#include <powsybl/iidm/VariantArray.hpp>

namespace powsybl {

namespace iidm {

class Network;

namespace network {

class VariantImpl : public Variant<Network, VariantImpl> {
public: // Variant
    std::unique_ptr<VariantImpl> copy() const override;

public:
    VariantImpl(Network& network);

    VariantImpl(const VariantImpl& variant) = delete;

    VariantImpl(VariantImpl&& variant) noexcept = delete;

    VariantImpl(Network& network, VariantImpl&& variant) noexcept;

    ~VariantImpl() noexcept override = default;

    VariantImpl& operator=(const VariantImpl& variant) = delete;

    VariantImpl& operator=(VariantImpl&& variant) noexcept = delete;
};

using VariantArray = iidm::VariantArray<Network, VariantImpl>;

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKVARIANT_HPP
