/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/ConnectablePositionAdder.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePosition.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

ConnectablePositionAdder::FeederAdder::FeederAdder(ConnectablePositionAdder& parent, ConnectablePosition::OptionalFeeder& feeder) :
    m_parent(parent),
    m_feeder(feeder) {
}

ConnectablePositionAdder& ConnectablePositionAdder::FeederAdder::add() {
    if (m_name.empty()) {
        throw PowsyblException("Feeder name is empty");
    }
    if (!m_order) {
        throw PowsyblException("Feeder order is not set");
    }
    if (!m_direction) {
        throw PowsyblException("Feeder direction is not set");
    }
    m_feeder = ConnectablePosition::Feeder(m_name, *m_order, *m_direction);
    return m_parent;
}

ConnectablePositionAdder::FeederAdder& ConnectablePositionAdder::FeederAdder::withDirection(const ConnectablePosition::Direction& direction) {
    m_direction = direction;
    return *this;
}

ConnectablePositionAdder::FeederAdder& ConnectablePositionAdder::FeederAdder::withName(const std::string& name) {
    m_name = name;
    return *this;
}

ConnectablePositionAdder::FeederAdder& ConnectablePositionAdder::FeederAdder::withOrder(unsigned long order) {
    m_order = order;
    return *this;
}

ConnectablePositionAdder::ConnectablePositionAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ConnectablePositionAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Connectable>(extendable)) {
        return std::unique_ptr<ConnectablePosition>(new ConnectablePosition(dynamic_cast<Connectable&>(extendable), m_feeder, m_feeder1, m_feeder2, m_feeder3));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Connectable>()));
}

ConnectablePositionAdder::FeederAdder ConnectablePositionAdder::newFeeder() {
    return FeederAdder(*this, m_feeder);
}

ConnectablePositionAdder::FeederAdder ConnectablePositionAdder::newFeeder1() {
    return FeederAdder(*this, m_feeder1);
}

ConnectablePositionAdder::FeederAdder ConnectablePositionAdder::newFeeder2() {
    return FeederAdder(*this, m_feeder2);
}

ConnectablePositionAdder::FeederAdder ConnectablePositionAdder::newFeeder3() {
    return FeederAdder(*this, m_feeder3);
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
