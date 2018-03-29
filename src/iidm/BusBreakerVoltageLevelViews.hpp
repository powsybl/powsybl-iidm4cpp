/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVIEWS_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVIEWS_HPP

#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/BusView.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;

namespace bus_breaker_voltage_level {

class BusBreakerViewImpl : public BusBreakerView {
public: // BusBreakerView
    Bus& getBus(const std::string& busId) const override;

    Bus& getBus1(const std::string& switchId) const override;

    Bus& getBus2(const std::string& switchId) const override;

    Switch& getSwitch(const std::string& switchId) const override;

    BusAdder newBus() override;

    SwitchAdder newSwitch() override;

    void removeAllBuses() override;

    void removeAllSwitches() override;

    void removeBus(const std::string& busId) override;

    void removeSwitch(const std::string& switchId) override;

public:
    explicit BusBreakerViewImpl(BusBreakerVoltageLevel& voltageLevel);

    virtual ~BusBreakerViewImpl() = default;

private:
    BusBreakerVoltageLevel& m_voltageLevel;
};

class BusViewImpl : public BusView {
public:
    Bus& getBus(const std::string& busId) const override;

public:
    explicit BusViewImpl(BusBreakerVoltageLevel& voltageLevel);

    virtual ~BusViewImpl() = default;

private:
    BusBreakerVoltageLevel& m_voltageLevel;
};

}

}

}

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVIEWS_HPP
