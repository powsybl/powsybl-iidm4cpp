/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "MergedBus.hpp"

#include <cassert>
#include <cmath>

#include <boost/range/join.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

MergedBus::MergedBus(const std::string& id, const std::string& name, BusSet buses) :
    Bus(id, name),
    m_buses(std::move(buses)),
    m_valid(true) {

    if (m_buses.empty()) {
        throw PowsyblException("The set of buses is empty");
    }
}

void MergedBus::checkValidity() const {
    if (!m_valid) {
        throw PowsyblException("Bus has been invalidated");
    }
}

double MergedBus::getAngle() const {
    checkValidity();

    for (const auto& it : m_buses) {
        if (!std::isnan(it.get().getAngle())) {
            return it.get().getAngle();
        }
    }
    return stdcxx::nan();
}

unsigned long MergedBus::getConnectedTerminalCount() const {
    checkValidity();

    unsigned long count = 0;
    for (const auto& it : m_buses) {
        count += it.get().getConnectedTerminalCount();
    }

    return count;
}

stdcxx::const_range<Terminal> MergedBus::getConnectedTerminals() const {
    checkValidity();

    stdcxx::const_range<Terminal> range;

    for (const auto& bus : m_buses) {
        range = boost::range::join(range, bus.get().getConnectedTerminals());
    }

    return range;
}

stdcxx::range<Terminal> MergedBus::getConnectedTerminals() {
    checkValidity();

    stdcxx::range<Terminal> range;

    for (const auto& bus : m_buses) {
        range = boost::range::join(range, bus.get().getConnectedTerminals());
    }

    return range;
}

double MergedBus::getV() const {
    for (const auto& it : m_buses) {
        if (!std::isnan(it.get().getV())) {
            return it.get().getV();
        }
    }
    return stdcxx::nan();
}

VoltageLevel& MergedBus::getVoltageLevel() const {
    checkValidity();
    assert(!m_buses.empty());

    return m_buses.begin()->get().getVoltageLevel();
}

void MergedBus::invalidate() {
    m_valid = false;
    m_buses.clear();
}

Bus& MergedBus::setAngle(double angle) {
    checkValidity();
    for (auto& it : m_buses) {
        it.get().setAngle(angle);
    }

    return *this;
}

Bus& MergedBus::setV(double v) {
    checkValidity();
    for (auto& it : m_buses) {
        it.get().setV(v);
    }

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
