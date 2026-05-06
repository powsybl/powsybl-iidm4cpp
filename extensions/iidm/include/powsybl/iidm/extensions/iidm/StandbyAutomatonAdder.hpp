/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_STANDBYAUTOMATONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_STANDBYAUTOMATONADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class StandbyAutomatonAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit StandbyAutomatonAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~StandbyAutomatonAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    StandbyAutomatonAdder(const StandbyAutomatonAdder&) = default;

    /**
     * Move constructor
     */
    StandbyAutomatonAdder(StandbyAutomatonAdder&&) = default;

    /**
     * Copy assignment operator
     */
    StandbyAutomatonAdder& operator=(const StandbyAutomatonAdder&) = delete;

    /**
     * Move assignment operator
     */
    StandbyAutomatonAdder& operator=(StandbyAutomatonAdder&&) = delete;

    StandbyAutomatonAdder& withB0(double b0);

    StandbyAutomatonAdder& withHighVoltageSetpoint(double highVoltageSetpoint);

    StandbyAutomatonAdder& withHighVoltageThreshold(double highVoltageThreshold);

    StandbyAutomatonAdder& withLowVoltageSetpoint(double lowVoltageSetpoint);

    StandbyAutomatonAdder& withLowVoltageThreshold(double lowVoltageThreshold);

    StandbyAutomatonAdder& withStandby(bool standby);

protected:
    /**
     * Creates the StandbyAutomaton extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    double m_b0 = 0.0;

    bool m_standby = false;

    double m_lowVoltageSetpoint = 0.0;

    double m_highVoltageSetpoint = 0.0;

    double m_lowVoltageThreshold = 0.0;

    double m_highVoltageThreshold = 0.0;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_STANDBYAUTOMATONADDER_HPP
