/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROLADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROLADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class HvdcAngleDroopActivePowerControlAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit HvdcAngleDroopActivePowerControlAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    HvdcAngleDroopActivePowerControlAdder(const HvdcAngleDroopActivePowerControlAdder&) = default;

    /**
     * Move constructor
     */
    HvdcAngleDroopActivePowerControlAdder(HvdcAngleDroopActivePowerControlAdder&&) = default;

    /**
     * Destructor
     */
    ~HvdcAngleDroopActivePowerControlAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    HvdcAngleDroopActivePowerControlAdder& operator=(const HvdcAngleDroopActivePowerControlAdder&) = delete;

    /**
     * Move assignment operator
     */
    HvdcAngleDroopActivePowerControlAdder& operator=(HvdcAngleDroopActivePowerControlAdder&&) = delete;

    HvdcAngleDroopActivePowerControlAdder& withDroop(double droop);

    HvdcAngleDroopActivePowerControlAdder& withEnabled(bool enabled);

    HvdcAngleDroopActivePowerControlAdder& withP0(double p0);

protected:
    /**
     * Creates the CoordinatedReactiveControl extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    /**
     * Active power offset in MW
     */
    double m_p0 = stdcxx::nan();

    /**
     * Droop in MW/degree
     */
    double m_droop = stdcxx::nan();

    /**
     * Enables or disables this active power control mode.
     * If this active power control mode is disabled, use the setpoint mode by default.
     */
    bool m_enabled = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROLADDER_HPP
