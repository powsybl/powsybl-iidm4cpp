/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class HvdcLine;

namespace extensions {

namespace iidm {

class HvdcAngleDroopActivePowerControl : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    double getDroop() const;

    double getP0() const;

    bool isEnabled() const;

    HvdcAngleDroopActivePowerControl& setDroop(double droop);

    HvdcAngleDroopActivePowerControl& setEnabled(bool enabled);

    HvdcAngleDroopActivePowerControl& setP0(double p0);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static double checkDroop(double droop);

    static double checkP0(double p0);

private:
    HvdcAngleDroopActivePowerControl(HvdcLine& hvdcLine, double p0, double droop, bool enabled);

    friend class HvdcAngleDroopActivePowerControlAdder;

private:
    /**
     * Active power offset in MW
     */
    double m_p0;

    /**
     * Droop in MW/degree
     */
    double m_droop;

    /**
     * Enables or disables this active power control mode.
     * If this active power control mode is disabled, use the setpoint mode by default.
     */
    bool m_enabled;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROL_HPP
