/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/ConnectablePosition.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<powsybl::iidm::extensions::sld::ConnectablePosition::Direction>() {
    static std::initializer_list<std::string> s_names = {
        "TOP",
        "BOTTOM",
        "UNDEFINED"
    };
    return s_names;
}

}  // namespace Enum

namespace extensions {

namespace sld {

ConnectablePosition::Feeder::Feeder(const std::string& name, unsigned long order, const Direction& direction) :
    m_name(name),
    m_order(order),
    m_direction(direction) {
}

const ConnectablePosition::Direction& ConnectablePosition::Feeder::getDirection() const {
    return m_direction;
}

const std::string& ConnectablePosition::Feeder::getName() const {
    return m_name;
}

unsigned long ConnectablePosition::Feeder::getOrder() const {
    return m_order;
}

ConnectablePosition::Feeder& ConnectablePosition::Feeder::setDirection(const Direction& direction) {
    m_direction = direction;
    return *this;
}

ConnectablePosition::Feeder& ConnectablePosition::Feeder::setName(const std::string& name) {
    m_name = name;
    return *this;
}

ConnectablePosition::Feeder& ConnectablePosition::Feeder::setOrder(unsigned long order) {
    m_order = order;
    return *this;
}

ConnectablePosition::ConnectablePosition(Connectable& connectable, const OptionalFeeder& feeder, const OptionalFeeder& feeder1, const OptionalFeeder& feeder2, const OptionalFeeder& feeder3) :
    Extension(connectable),
    m_feeder(feeder),
    m_feeder1(feeder1),
    m_feeder2(feeder2),
    m_feeder3(feeder3) {
    check(feeder, feeder1, feeder2, feeder3);
}

void ConnectablePosition::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Connectable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Connectable>()));
    }
}

void ConnectablePosition::check(const OptionalFeeder& feeder, const OptionalFeeder& feeder1, const OptionalFeeder& feeder2, const OptionalFeeder& feeder3) {
    if (!feeder && !feeder1 && !feeder2 && !feeder3) {
        throw PowsyblException(stdcxx::format("invalid feeder"));
    }
    if (feeder && (feeder1 || feeder2 || feeder3)) {
        throw PowsyblException(stdcxx::format("feeder and feeder 1|2|3 are exclusives"));
    }
    bool error = false;
    if (feeder1) {
        if (!feeder2 && feeder3) {
            error = true;
        }
    } else {
        if (feeder2 || feeder3) {
            error = true;
        }
    }
    if (error) {
        throw PowsyblException(stdcxx::format("feeder 1|2|3 have to be set in the right order"));
    }
}

stdcxx::CReference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder() const {
    return m_feeder.is_initialized() ? stdcxx::cref(*m_feeder) : stdcxx::cref<Feeder>();
}

stdcxx::Reference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder() {
    return m_feeder.is_initialized() ? stdcxx::ref(*m_feeder) : stdcxx::ref<Feeder>();
}

stdcxx::CReference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder1() const {
    return m_feeder1.is_initialized() ? stdcxx::cref(*m_feeder1) : stdcxx::cref<Feeder>();
}

stdcxx::Reference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder1() {
    return m_feeder1.is_initialized() ? stdcxx::ref(*m_feeder1) : stdcxx::ref<Feeder>();
}

stdcxx::CReference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder2() const {
    return m_feeder2.is_initialized() ? stdcxx::cref(*m_feeder2) : stdcxx::cref<Feeder>();
}

stdcxx::Reference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder2() {
    return m_feeder2.is_initialized() ? stdcxx::ref(*m_feeder2) : stdcxx::ref<Feeder>();
}

stdcxx::CReference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder3() const {
    return m_feeder3.is_initialized() ? stdcxx::cref(*m_feeder3) : stdcxx::cref<Feeder>();
}

stdcxx::Reference<ConnectablePosition::Feeder> ConnectablePosition::getFeeder3() {
    return m_feeder3.is_initialized() ? stdcxx::ref(*m_feeder3) : stdcxx::ref<Feeder>();
}

const std::string& ConnectablePosition::getName() const {
    static std::string s_name = "position";
    return s_name;
}

const std::type_index& ConnectablePosition::getType() const {
    static std::type_index s_type = typeid(ConnectablePosition);
    return s_type;
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
