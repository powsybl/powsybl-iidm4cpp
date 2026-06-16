/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GENERATOR_HPP
#define POWSYBL_IIDM_GENERATOR_HPP

#include <functional>

#include <powsybl/iidm/EnergySource.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/Referrer.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

class Generator : public Injection, public ReactiveLimitsHolder, public Referrer<Terminal> {
public:  // Identifiable
    const IdentifiableType& getType() const override;

public: //Connectable
    virtual void remove() override;

public: //Referrer<Terminal>
    void onReferencedRemoval(Terminal& removedReference) override;

    void onReferencedReplacement(Terminal& oldReference, Terminal& newReference) override;

public:
    Generator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, const EnergySource& energySource,
        double minP, double maxP, bool voltageRegulatorOn, stdcxx::Reference<Terminal>& regulatingTerminal,
        double activePowerSetpoint, double reactivePowerSetpoint, double voltageSetpoint, double equivalentLocalTargetV, double ratedS, bool isCondenser);

    ~Generator() noexcept override = default;

    double getActivePowerSetpoint() const;

    const EnergySource& getEnergySource() const;

    double getMaxP() const;

    double getMinP() const;

    double getRatedS() const;

    double getReactivePowerSetpoint() const;

    const Terminal& getRegulatingTerminal() const;

    Terminal& getRegulatingTerminal();

    double getTargetP() const;

    double getTargetQ() const;

    double getTargetV() const;

    double getVoltageSetpoint() const;

    double getEquivalentLocalTargetV() const;

    bool isVoltageRegulatorOn() const;

    bool isCondenser() const;

    Generator& setActivePowerSetpoint(double activePowerSetpoint);

    Generator& setEnergySource(const EnergySource& energySource);

    Generator& setMaxP(double maxP);

    Generator& setMinP(double minP);

    Generator& setRatedS(double ratedS);

    Generator& setReactivePowerSetpoint(double reactivePowerSetpoint);

    Generator& setRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal);

    Generator& setTargetP(double activePowerSetpoint);

    Generator& setTargetQ(double reactivePowerSetpoint);

    /**
     * Set the voltage target for the regulated terminal (local or remote) and the optional equivalentLocalTargetV (to NaN if not specified)
     */
    Generator& setTargetV(double voltageSetpoint, double equivalentLocalTargetV = stdcxx::nan());

    Generator& setVoltageRegulatorOn(bool voltageRegulatorOn);

    Generator& setVoltageSetpoint(double voltageSetpoint, double equivalentLocalTargetV = stdcxx::nan());

    void applySolvedValues();
    void setTargetPToP();
    void setTargetQToQ();
    void setTargetVToV();

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    EnergySource m_energySource;

    double m_minP;

    double m_maxP;

    double m_ratedS;

    stdcxx::Reference<Terminal> m_regulatingTerminal;

    std::vector<bool> m_voltageRegulatorOn;

    std::vector<double> m_activePowerSetpoint;

    std::vector<double> m_reactivePowerSetpoint;

    std::vector<double> m_voltageSetpoint;
    std::vector<double> m_equivalentLocalTargetV;

    bool m_isCondenser;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GENERATOR_HPP
