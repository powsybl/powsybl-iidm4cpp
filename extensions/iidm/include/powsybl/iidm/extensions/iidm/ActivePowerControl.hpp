/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Injection;
class Battery;
class Generator;

namespace extensions {

namespace iidm {

class ActivePowerControl : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~ActivePowerControl() noexcept override = default;

    double getDroop() const;

    bool isParticipate() const;

    double getParticipationFactor() const;

    double getMinTargetP() const;

    double getMaxTargetP() const;

    ActivePowerControl& setDroop(double droop);

    ActivePowerControl& setParticipate(bool participate);

    ActivePowerControl& setParticipationFactor(double participationFactor);

    ActivePowerControl& setMinTargetP(double minTargetP);

    ActivePowerControl& setMaxTargetP(double maxTargetP);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    ActivePowerControl(Battery& battery, bool participate, double droop, double participationFactor, double minTargetP = stdcxx::nan(), double maxTargetP = stdcxx::nan());

    ActivePowerControl(Generator& generator, bool participate, double droop, double participationFactor, double minTargetP = stdcxx::nan(), double maxTargetP = stdcxx::nan());

    friend class ActivePowerControlAdder;

    struct PLimits {
        double m_minP;
        double m_maxP;
    };
    PLimits getPLimits(const Injection& extendedComponent) const ;

    double checkWithinPMinMax(double value, const Injection& extendedComponent) const;
    double checkTargetPLimit(double targetPLimit, const std::string& name, const Injection& extendedComponent) const;
    void checkLimitOrder(double minTargetP, double maxTargetP) const;

private:
    bool m_participate;

    double m_droop;

    double m_participationFactor;

    double m_minTargetP;
    double m_maxTargetP;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROL_HPP
