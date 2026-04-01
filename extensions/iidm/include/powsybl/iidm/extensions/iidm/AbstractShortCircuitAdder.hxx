/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuitAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

template<typename A>
AbstractShortCircuitAdder<A>::AbstractShortCircuitAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

template<typename A>
A& AbstractShortCircuitAdder<A>::withDirectSubtransX(double directSubtransX) {
    m_directSubtransX = directSubtransX;
    return static_cast<A&>(*this);
}

template<typename A>
A& AbstractShortCircuitAdder<A>::withDirectTransX(double directTransX) {
    m_directTransX = directTransX;
    return static_cast<A&>(*this);
}

template<typename A>
A& AbstractShortCircuitAdder<A>::withStepUpTransformerX(double stepUpTransformerX) {
    m_stepUpTransformerX = stepUpTransformerX;
    return static_cast<A&>(*this);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
