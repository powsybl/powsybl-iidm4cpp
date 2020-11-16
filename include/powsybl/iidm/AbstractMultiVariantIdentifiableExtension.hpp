/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTMULTIVARIANTIDENTIFIABLEEXTENSION_HPP
#define POWSYBL_IIDM_ABSTRACTMULTIVARIANTIDENTIFIABLEEXTENSION_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>

namespace powsybl {

namespace iidm {

class Extendable;
class VariantManagerHolder;

class AbstractMultiVariantIdentifiableExtension : public Extension, public MultiVariantObject {
protected:
    explicit AbstractMultiVariantIdentifiableExtension(Extendable& extendable);

    unsigned long getVariantIndex() const;

    const VariantManagerHolder& getVariantManagerHolder() const;

    VariantManagerHolder& getVariantManagerHolder();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTMULTIVARIANTIDENTIFIABLEEXTENSION_HPP
