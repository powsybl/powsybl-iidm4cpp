/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VariantContextGuard.hpp>

#include <powsybl/iidm/VariantContext.hpp>

namespace powsybl {

namespace iidm {

VariantContextGuard::VariantContextGuard(VariantContext& context) :
    m_context(context),
    m_variantIndex(context.getVariantIndex()) {
}

VariantContextGuard::~VariantContextGuard() noexcept {
    m_context.setVariantIndex(m_variantIndex);
}

}  // namespace iidm

}  // namespace powsybl
