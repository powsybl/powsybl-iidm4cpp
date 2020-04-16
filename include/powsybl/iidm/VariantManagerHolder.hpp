/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANTMANAGERHOLDER_HPP
#define POWSYBL_IIDM_VARIANTMANAGERHOLDER_HPP

namespace powsybl {

namespace iidm {

class VariantManager;

class VariantManagerHolder {
public:
    virtual ~VariantManagerHolder() noexcept = default;

    virtual unsigned long getVariantIndex() const = 0;

    virtual const VariantManager& getVariantManager() const = 0;

    virtual VariantManager& getVariantManager() = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANTMANAGERHOLDER_HPP
