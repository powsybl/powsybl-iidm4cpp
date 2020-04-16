/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANTCONTEXT_HPP
#define POWSYBL_IIDM_VARIANTCONTEXT_HPP

namespace powsybl {

namespace iidm {

class VariantManager;

class VariantContext {
public:
    virtual ~VariantContext() noexcept = default;

protected:
    virtual unsigned long getVariantIndex() const = 0;

    virtual void resetIfVariantIndexIs(unsigned long index) = 0;

    virtual void setVariantIndex(unsigned long index) = 0;

private:
    friend class VariantContextGuard;

    friend class VariantManager;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANTCONTEXT_HPP
