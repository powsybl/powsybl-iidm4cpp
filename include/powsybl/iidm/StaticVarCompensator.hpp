/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATICVARCOMPENSATOR_HPP
#define POWSYBL_IIDM_STATICVARCOMPENSATOR_HPP

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

class StaticVarCompensator : public Injection {
public:
    enum class RegulationMode : unsigned int {
        VOLTAGE,
        REACTIVE_POWER,
        OFF
    };

public:
    StaticVarCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name,
                         double bMin, double bMax, double voltageSetpoint, double reactivePowerSetpoint, const RegulationMode& regulationMode);

    ~StaticVarCompensator() noexcept override = default;

    double getBmax() const;

    double getBmin() const;

    double getReactivePowerSetpoint() const;

    const RegulationMode& getRegulationMode() const;

    double getVoltageSetpoint() const;

    StaticVarCompensator& setBmax(double bMax);

    StaticVarCompensator& setBmin(double bMin);

    StaticVarCompensator& setReactivePowerSetpoint(double reactivePowerSetpoint);

    StaticVarCompensator& setRegulationMode(const RegulationMode& regulationMode);

    StaticVarCompensator& setVoltageSetpoint(double voltageSetpoint);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<VariantManagerHolder> m_network;

    double m_bMin;

    double m_bMax;

    std::vector<double> m_voltageSetpoint;

    std::vector<double> m_reactivePowerSetpoint;

    std::vector<RegulationMode> m_regulationMode;
};

std::ostream& operator<<(std::ostream& stream, const StaticVarCompensator::RegulationMode& mode);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_STATICVARCOMPENSATOR_HPP
