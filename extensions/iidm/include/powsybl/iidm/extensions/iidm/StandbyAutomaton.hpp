/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_STANDBYAUTOMATON_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_STANDBYAUTOMATON_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class StaticVarCompensator;

namespace extensions {

namespace iidm {

class StandbyAutomaton : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    bool isStandby() const;
    StandbyAutomaton& setStandby(bool standby);

    double getB0() const;
    StandbyAutomaton& setB0(double b0);

    double getHighVoltageSetpoint() const;
    StandbyAutomaton& setHighVoltageSetpoint(double highVoltageSetpoint);

    double getHighVoltageThreshold() const;
    StandbyAutomaton& setHighVoltageThreshold(double highVoltageThreshold);

    double getLowVoltageSetpoint() const;
    StandbyAutomaton& setLowVoltageSetpoint(double lowVoltageSetpoint);

    double getLowVoltageThreshold() const;
    StandbyAutomaton& setLowVoltageThreshold(double lowVoltageThreshold);


private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    StandbyAutomaton(StaticVarCompensator& svc, double b0, bool standby,
                                double lowVoltageSetpoint, double highVoltageSetpoint,
                                double lowVoltageThreshold, double highVoltageThreshold);

    double checkB0(double b0);
    void checkVoltageConfig(double lowVoltageSetpoint, double highVoltageSetpoint, double lowVoltageThreshold, double highVoltageThreshold);

    friend class StandbyAutomatonAdder;

private:
    double m_b0;

    bool m_standby;

    double m_lowVoltageSetpoint;
    double m_highVoltageSetpoint;
    double m_lowVoltageThreshold;
    double m_highVoltageThreshold;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_STANDBYAUTOMATON_HPP
