/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MULTIPLEVARIANTCONTEXT_HPP
#define POWSYBL_IIDM_MULTIPLEVARIANTCONTEXT_HPP

#include <powsybl/iidm/VariantContext.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class MultipleVariantContext : public VariantContext {
public:
    explicit MultipleVariantContext(unsigned long index);

    MultipleVariantContext(const MultipleVariantContext&) = default;

    MultipleVariantContext(MultipleVariantContext&&) noexcept = default;

    ~MultipleVariantContext() noexcept override = default;

    MultipleVariantContext& operator=(const MultipleVariantContext&) = default;

    MultipleVariantContext& operator=(MultipleVariantContext&&) noexcept = default;

protected: // VariantContext
    unsigned long getVariantIndex() const override;

    bool isIndexSet() const override;

    void resetIfVariantIndexIs(unsigned long index) override;

    void setVariantIndex(unsigned long index) override;

private:
    stdcxx::optional<unsigned long> m_index;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_MULTIPLEVARIANTCONTEXT_HPP
