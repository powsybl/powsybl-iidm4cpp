/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATICVARCOMPENSATORADDER_HPP
#define POWSYBL_IIDM_STATICVARCOMPENSATORADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class StaticVarCompensatorAdder : public InjectionAdder<StaticVarCompensatorAdder> {
public:
    ~StaticVarCompensatorAdder() noexcept override = default;

    StaticVarCompensator& add();

    StaticVarCompensatorAdder& setBmax(double bMax);

    StaticVarCompensatorAdder& setBmin(double bMin);

    StaticVarCompensatorAdder& setReactivePowerSetpoint(double reactivePowerSetpoint);

    StaticVarCompensatorAdder& setRegulationMode(const StaticVarCompensator::RegulationMode& regulationMode);

    StaticVarCompensatorAdder& setVoltageSetpoint(double voltageSetpoint);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit StaticVarCompensatorAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    VoltageLevel& m_voltageLevel;

    double m_bMin = stdcxx::nan();

    double m_bMax = stdcxx::nan();

    double m_voltageSetpoint = stdcxx::nan();

    double m_reactivePowerSetpoint = stdcxx::nan();

    stdcxx::optional<StaticVarCompensator::RegulationMode> m_regulationMode;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_STATICVARCOMPENSATORADDER_HPP
