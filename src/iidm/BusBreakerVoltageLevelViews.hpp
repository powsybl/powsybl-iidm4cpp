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
#include <powsybl/math/UndirectedGraph.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;

namespace bus_breaker_voltage_level {

class BusBreakerViewImpl : public BusBreakerView {
public: // BusBreakerView
    stdcxx::Reference<Bus> getBus(const std::string& busId) const override;

    stdcxx::Reference<Bus> getBus1(const std::string& switchId) const override;

    stdcxx::Reference<Bus> getBus2(const std::string& switchId) const override;

    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const override;

    unsigned long getSwitchCount() const override;

    BusAdder newBus() override;

    SwitchAdder newSwitch() override;

    void removeAllBuses() override;

    void removeAllSwitches() override;

    void removeBus(const std::string& busId) override;

    void removeSwitch(const std::string& switchId) override;

public:
    explicit BusBreakerViewImpl(BusBreakerVoltageLevel& voltageLevel);

    ~BusBreakerViewImpl() noexcept override = default;

private:
    BusBreakerVoltageLevel& m_voltageLevel;
};

class BusViewImpl : public BusView {
public:
    stdcxx::Reference<Bus> getBus(const std::string& busId) const override;

    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

    stdcxx::Reference<Bus> getMergedBus(const std::string& configuredBusId) const override;

public:
    explicit BusViewImpl(BusBreakerVoltageLevel& voltageLevel);

    ~BusViewImpl() noexcept override = default;

private:
    BusBreakerVoltageLevel& m_voltageLevel;
};

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVIEWS_HPP
