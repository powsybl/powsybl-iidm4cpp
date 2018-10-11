/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GENERATORADDER_HPP
#define POWSYBL_IIDM_GENERATORADDER_HPP

#include <powsybl/iidm/EnergySource.hpp>
#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Generator;
class Network;
class Terminal;
class VoltageLevel;

class GeneratorAdder : public InjectionAdder<GeneratorAdder> {
public:
    ~GeneratorAdder() noexcept override = default;

    Generator& add();

    GeneratorAdder& setActivePowerSetpoint(double activePowerSetpoint);

    GeneratorAdder& setEnergySource(const EnergySource& energySource);

    GeneratorAdder& setMaxP(double maxP);

    GeneratorAdder& setMinP(double minP);

    GeneratorAdder& setRatedS(double ratedS);

    GeneratorAdder& setReactivePowerSetpoint(double reactivePowerSetpoint);

    GeneratorAdder& setRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal);

    GeneratorAdder& setTargetP(double activePowerSetpoint);

    GeneratorAdder& setTargetQ(double reactivePowerSetpoint);

    GeneratorAdder& setTargetV(double voltageSetpoint);

    GeneratorAdder& setVoltageRegulatorOn(bool voltageRegulatorOn);

    GeneratorAdder& setVoltageSetpoint(double voltageSetpoint);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit GeneratorAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    VoltageLevel& m_voltageLevel;

    EnergySource m_energySource;

    double m_minP;

    double m_maxP;

    double m_ratedS;

    stdcxx::Reference<Terminal> m_regulatingTerminal;

    stdcxx::optional<bool> m_voltageRegulatorOn;

    double m_activePowerSetpoint;

    double m_reactivePowerSetpoint;

    double m_voltageSetpoint;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GENERATORADDER_HPP
