/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VSCCONVERTERSTATION_HPP
#define POWSYBL_IIDM_VSCCONVERTERSTATION_HPP

#include <functional>

#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

class VscConverterStation : public HvdcConverterStation, public ReactiveLimitsHolder {
public: // HvdcConverterStation
    HvdcType getHvdcType() const override;

    VscConverterStation& setLossFactor(double lossFactor) override;

public:
    VscConverterStation(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, double lossFactor, bool voltageRegulatorOn, double reactivePowerSetpoint, double voltageSetpoint, Terminal& regulatingTerminal);

    ~VscConverterStation() noexcept override = default;

    double getReactivePowerSetpoint() const;

    const Terminal& getRegulatingTerminal() const;

    Terminal& getRegulatingTerminal();

    double getVoltageSetpoint() const;

    bool isVoltageRegulatorOn() const;

    VscConverterStation& setReactivePowerSetpoint(double reactivePowerSetpoint);

    VscConverterStation& setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal);

    VscConverterStation& setVoltageRegulatorOn(bool voltageRegulatorOn);

    VscConverterStation& setVoltageSetpoint(double voltageSetpoint);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    std::vector<bool> m_voltageRegulatorOn;

    std::vector<double> m_reactivePowerSetpoint;

    std::vector<double> m_voltageSetpoint;

    std::reference_wrapper<Terminal> m_regulatingTerminal;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VSCCONVERTERSTATION_HPP
