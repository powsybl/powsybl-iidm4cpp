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

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

MergedBus::MergedBus(const std::string& id, BusSet buses) :
    Bus(id),
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

stdcxx::CReference<Component> MergedBus::getConnectedComponent() const {
    checkValidity();

    for (const auto& it : m_buses) {
        const auto& cc = it.get().getConnectedComponent();
        if (static_cast<bool>(cc)) {
            return stdcxx::cref<Component>(cc);
        }
    }

    throw AssertionError("Should not happen");
}

stdcxx::Reference<Component> MergedBus::getConnectedComponent() {
    const auto& comp = static_cast<const MergedBus*>(this)->getConnectedComponent();

    return stdcxx::ref(comp);
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

stdcxx::CReference<Component> MergedBus::getSynchronousComponent() const {
    checkValidity();

    for (const auto& it : m_buses) {
        const auto& sc = it.get().getSynchronousComponent();
        if (static_cast<bool>(sc)) {
            return stdcxx::cref<Component>(sc);
        }
    }

    throw AssertionError("Should not happen");
}

stdcxx::Reference<Component> MergedBus::getSynchronousComponent() {
    const auto& comp = static_cast<const MergedBus*>(this)->getSynchronousComponent();

    return stdcxx::ref(comp);
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

bool MergedBus::isInMainConnectedComponent() const {
    if (!m_buses.empty()) {
        return m_buses.front().get().isInMainConnectedComponent();
    }

    return false;
}

bool MergedBus::isInMainSynchronousComponent() const {
    if (!m_buses.empty()) {
        return m_buses.front().get().isInMainSynchronousComponent();
    }

    return false;
}

Bus& MergedBus::setAngle(double angle) {
    checkValidity();
    for (auto& it : m_buses) {
        it.get().setAngle(angle);
    }

    return *this;
}

void MergedBus::setConnectedComponentNumber(long connectedComponentNumber) {
    checkValidity();
    for (auto& it : m_buses) {
        it.get().setConnectedComponentNumber(connectedComponentNumber);
    }
}

void MergedBus::setSynchronousComponentNumber(long componentNumber) {
    checkValidity();
    for (auto& it : m_buses) {
        it.get().setSynchronousComponentNumber(componentNumber);
    }
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
