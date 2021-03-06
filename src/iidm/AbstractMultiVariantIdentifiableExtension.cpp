/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

AbstractMultiVariantIdentifiableExtension::AbstractMultiVariantIdentifiableExtension(Extendable& extendable) :
    Extension(extendable) {
}

unsigned long AbstractMultiVariantIdentifiableExtension::getVariantIndex() const {
    return getVariantManagerHolder().getVariantIndex();
}

const VariantManagerHolder& AbstractMultiVariantIdentifiableExtension::getVariantManagerHolder() const {
    return getExtendable<Identifiable>().get().getNetwork();
}

VariantManagerHolder& AbstractMultiVariantIdentifiableExtension::getVariantManagerHolder() {
    return getExtendable<Identifiable>().get().getNetwork();
}

}  // namespace iidm

}  // namespace powsybl
