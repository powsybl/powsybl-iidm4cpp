/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGESOURCECONVERTER_HPP
#define POWSYBL_IIDM_VOLTAGESOURCECONVERTER_HPP

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

class VoltageSourceConverter : public AcDcConverter, public ReactiveLimitsHolder {

public:
    ~VoltageSourceConverter() noexcept override = default;

public:  // Identifiable
    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

public: // AcDcConverter setter for VoltageSourceConverter chaining
    VoltageSourceConverter& setIdleLoss(double idleLoss);
    VoltageSourceConverter& setSwitchingLoss(double switchingLoss);
    VoltageSourceConverter& setResistiveLoss(double resistiveLoss);
    VoltageSourceConverter& setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal);
    VoltageSourceConverter& setControlMode(const ControlMode& controlMode);
    VoltageSourceConverter& setTargetP(double targetP);
    VoltageSourceConverter& setTargetVdc(double targetVdc);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public: //Referrer<Terminal> 
    virtual void onReferencedRemoval(Terminal& removedReference) override;

public:
    /**
     * Check if voltage regulator is on.
     */
    bool isVoltageRegulatorOn();
    /**
     * Set voltage regulator status.
     */
    VoltageSourceConverter& setVoltageRegulatorOn(bool voltageRegulatorOn);

    /**
     * Get the AC voltage setpoint (kV).
     */
    double getVoltageSetpoint();
    /**
     * Set the AC voltage setpoint (kV).
     */
    VoltageSourceConverter& setVoltageSetpoint(double voltageSetpoint);

    /**
     * Get the reactive power setpoint (MVar).
     */
    double getReactivePowerSetpoint();
    /**
     * Set the reactive power setpoint (MVar).
     */
    VoltageSourceConverter& setReactivePowerSetpoint(double reactivePowerSetpoint);

protected:
    VoltageSourceConverter(const std::string& id, const std::string& name, bool fictitious, unsigned long variantArraySize, 
                    double idleLoss, double switchingLoss, double resistiveLoss, 
                    const stdcxx::Reference<Terminal>& pccTerminal, const ControlMode& controlMode, double targetP, double targetVdc,
                    bool voltageRegulatorOn, double reactivePowerSetpoint, double voltageSetpoint);
    friend class VoltageSourceConverterAdder;

private:

    std::vector<bool> m_voltageRegulatorOn;
    std::vector<double> m_reactivePowerSetpoint;
    std::vector<double> m_voltageSetpoint;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGESOURCECONVERTER_HPP
