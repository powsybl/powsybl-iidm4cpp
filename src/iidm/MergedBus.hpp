/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MERGEDBUS_HPP
#define POWSYBL_IIDM_MERGEDBUS_HPP

#include <functional>
#include <set>
#include <string>
#include <vector>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>
#include <powsybl/stdcxx/set.hpp>

#include "ConfiguredBus.hpp"

namespace powsybl {

namespace iidm {

/**
 * A Bus implementation used in the BusView of a BusBreakerVoltageLevel
 */
class MergedBus : public Bus {
public:
    using BusSet = stdcxx::LinkedHashSet<std::reference_wrapper<ConfiguredBus>, stdcxx::hash<std::reference_wrapper<ConfiguredBus> >, stdcxx::equal_to<ConfiguredBus> >;

public: // Bus
    double getAngle() const override;

    unsigned long getConnectedTerminalCount() const override;

    stdcxx::const_range<Terminal> getConnectedTerminals() const override;

    stdcxx::range<Terminal> getConnectedTerminals() override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public:
    /**
     * Create a new MergedBus from a list of ConfiguredBus references
     * @param id the ID of this bus
     * @param name the name of this bus
     * @param buses the list of ConfiguredBus aggregated in this bus
     */
    MergedBus(const std::string& id, const std::string& name, BusSet buses);

    ~MergedBus() noexcept override = default;

    /**
     * Invalidate this bus after the voltage level topology changed
     */
    void invalidate();

private:
    void checkValidity() const;

private:
    BusSet m_buses;

    bool m_valid;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_MERGEDBUS_HPP
