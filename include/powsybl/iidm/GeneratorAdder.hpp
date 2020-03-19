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
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Generator;
class Network;
class Terminal;
class VoltageLevel;

class IIDM_DECLSPEC GeneratorAdder : public InjectionAdder<GeneratorAdder> {
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

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit GeneratorAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    EnergySource m_energySource = EnergySource::OTHER;

    double m_minP = stdcxx::nan();

    double m_maxP = stdcxx::nan();

    double m_ratedS = stdcxx::nan();

    stdcxx::Reference<Terminal> m_regulatingTerminal;

    stdcxx::optional<bool> m_voltageRegulatorOn;

    double m_activePowerSetpoint = stdcxx::nan();

    double m_reactivePowerSetpoint = stdcxx::nan();

    double m_voltageSetpoint = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GENERATORADDER_HPP
