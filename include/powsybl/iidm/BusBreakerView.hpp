/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVIEW_HPP
#define POWSYBL_IIDM_BUSBREAKERVIEW_HPP

#include <string>

#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Switch;

class BusBreakerView {
public:
    using SwitchAdder = bus_breaker_view::SwitchAdder;

public:
    virtual ~BusBreakerView() noexcept = default;

    virtual stdcxx::Reference<Bus> getBus(const std::string& busId) const = 0;

    virtual stdcxx::Reference<Bus> getBus1(const std::string& switchId) const = 0;

    virtual stdcxx::Reference<Bus> getBus2(const std::string& switchId) const = 0;

    virtual stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const = 0;

    virtual unsigned long getSwitchCount() const = 0;

    virtual BusAdder newBus() = 0;

    virtual SwitchAdder newSwitch() = 0;

    virtual void removeAllBuses() = 0;

    virtual void removeAllSwitches() = 0;

    virtual void removeBus(const std::string& busId) = 0;

    virtual void removeSwitch(const std::string& switchId) = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVIEW_HPP
