/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AbstractMultiVariantConnectableExtension.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

AbstractMultiVariantConnectableExtension::AbstractMultiVariantConnectableExtension(Extendable& extendable) :
    Extension(extendable) {
}

unsigned long AbstractMultiVariantConnectableExtension::getVariantIndex() const {
    return getVariantManagerHolder().getVariantIndex();
}

const VariantManagerHolder& AbstractMultiVariantConnectableExtension::getVariantManagerHolder() const {
    const Connectable& connectable = dynamic_cast<const Connectable&>(getExtendable().get());

    return getVariantManagerHolder(connectable);
}

const VariantManagerHolder& AbstractMultiVariantConnectableExtension::getVariantManagerHolder(const Connectable& connectable) {
    const Network& network = dynamic_cast<const Terminal&>(connectable.getTerminals()[0].get()).getVoltageLevel().getSubstation().getNetwork();

    if (stdcxx::isInstanceOf<VariantManagerHolder>(network)) {
        return dynamic_cast<const VariantManagerHolder&>(network);
    }

    throw PowsyblException(logging::format("Network cannot be converted to VariantManagerHolder"));
}

}  // namespace iidm

}  // namespace powsybl