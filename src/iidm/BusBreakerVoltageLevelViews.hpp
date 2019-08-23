/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVIEWS_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELVIEWS_HPP

#include <powsybl/iidm/VoltageLevelViews.hpp>
#include <powsybl/math/UndirectedGraph.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;

namespace bus_breaker_voltage_level {

class BusBreakerViewImpl : public voltage_level::BusBreakerView {
public: // BusBreakerView
    stdcxx::CReference<Bus> getBus(const std::string& busId) const override;

    stdcxx::Reference<Bus> getBus(const std::string& busId) override;

    stdcxx::CReference<Bus> getBus1(const std::string& switchId) const override;

    stdcxx::Reference<Bus> getBus1(const std::string& switchId) override;

    stdcxx::CReference<Bus> getBus2(const std::string& switchId) const override;

    stdcxx::Reference<Bus> getBus2(const std::string& switchId) override;

    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

    stdcxx::CReference<Switch> getSwitch(const std::string& switchId) const override;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) override;

    unsigned long getSwitchCount() const override;

    stdcxx::const_range<Switch> getSwitches() const override;

    stdcxx::range<Switch> getSwitches() override;

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

class BusViewImpl : public voltage_level::BusView {
public:
    stdcxx::CReference<Bus> getBus(const std::string& busId) const override;

    stdcxx::Reference<Bus> getBus(const std::string& busId) override;

    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

    stdcxx::CReference<Bus> getMergedBus(const std::string& configuredBusId) const override;

    stdcxx::Reference<Bus> getMergedBus(const std::string& configuredBusId) override;

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
