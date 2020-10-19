/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Component.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NetworkViews.hpp>

namespace powsybl {

namespace iidm {

Component::Component(unsigned long num, unsigned long size) :
    m_num(num),
    m_size(size) {
}

stdcxx::const_range<Bus> Component::getBuses() const {
    return getNetwork().getBusView().getBuses() | boost::adaptors::filtered(getBusPredicate());
}

stdcxx::range<Bus> Component::getBuses() {
    return getNetwork().getBusView().getBuses() | boost::adaptors::filtered(getBusPredicate());
}

unsigned long Component::getNum() const {
    return m_num;
}

unsigned long Component::getSize() const {
    return m_size;
}

}  // namespace iidm

}  // namespace powsybl
