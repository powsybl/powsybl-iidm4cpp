/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_STANDBYAUTOMATON_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_STANDBYAUTOMATON_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class StaticVarCompensator;

namespace extensions {

namespace cvg {

class StandbyAutomaton : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    StandbyAutomaton(StaticVarCompensator& svc, double b0, bool standby, double lowVoltageSetpoint, double highVoltageSetpoint,
                     double lowVoltageThreshold, double highVoltageThreshold);

    double getB0() const;

    double getHighVoltageSetpoint() const;

    double getHighVoltageThreshold() const;

    double getLowVoltageSetpoint() const;

    double getLowVoltageThreshold() const;

    bool isStandby() const;

    StandbyAutomaton& setB0(double b0);

    StandbyAutomaton& setHighVoltageSetpoint(double highVoltageSetpoint);

    StandbyAutomaton& setHighVoltageThreshold(double highVoltageThreshold);

    StandbyAutomaton& setLowVoltageSetpoint(double lowVoltageSetpoint);

    StandbyAutomaton& setLowVoltageThreshold(double lowVoltageThreshold);

    StandbyAutomaton& setStandby(bool standby);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static double checkB0(double v);

    static void checkVoltageConfig(double lowVoltageSetpoint, double highVoltageSetpoint, double lowVoltageThreshold, double highVoltageThreshold);

private:
    double m_b0;

    bool m_standby;

    double m_lowVoltageSetpoint;

    double m_highVoltageSetpoint;

    double m_lowVoltageThreshold;

    double m_highVoltageThreshold;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_STANDBYAUTOMATON_HPP

