/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANTCONTEXTGUARD_HPP
#define POWSYBL_IIDM_VARIANTCONTEXTGUARD_HPP

#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class VariantContext;

/**
 * A scope guard implementation based on RAII idiom to restore the variant index
 */
class IIDM_DECLSPEC VariantContextGuard {
public:
    explicit VariantContextGuard(VariantContext& context);

    VariantContextGuard(const VariantContextGuard& guard) = delete;

    ~VariantContextGuard() noexcept;

    VariantContextGuard& operator=(const VariantContextGuard& guard) = delete;

private:
    VariantContext& m_context;

    unsigned long m_variantIndex;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANTCONTEXTGUARD_HPP
