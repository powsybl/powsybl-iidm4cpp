/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ACDCCONVERTER_HPP
#define POWSYBL_IIDM_ACDCCONVERTER_HPP

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/DcConnectable.hpp>
#include <powsybl/iidm/Referrer.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class AcDcConverter : public Connectable, public DcConnectable, public Referrer<Terminal> {

public:
    enum class ControlMode : unsigned char {
        P_PCC,
        V_DC
    };

protected:
    AcDcConverter(unsigned long variantArraySize,
                    double idleLoss, double switchingLoss, double resistiveLoss, 
                    const stdcxx::Reference<Terminal>& pccTerminal, const ControlMode& controlMode,
                    double targetP, double targetVdc);
public:
    ~AcDcConverter() noexcept override = default;

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

protected: // MultiVariantObject
    virtual void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    virtual void deleteVariantArrayElement(unsigned long index) override;

    virtual void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    virtual void reduceVariantArraySize(unsigned long number) override;

public: //Referrer<Terminal> 
    virtual void onReferencedRemoval(Terminal& removedReference) override;
    virtual void onReferencedReplacement(Terminal& oldReference, Terminal& newReference) override;

public:
    /**
     * Get the first AC Terminal
     */
    const Terminal& getTerminal1() const;
    /**
     * Get the first AC Terminal
     */
    Terminal& getTerminal1();

    /**
     * Get the optional second AC Terminal
     */
    stdcxx::CReference<Terminal> getTerminal2() const;
    /**
     * Get the optional second AC Terminal
     */
    stdcxx::Reference<Terminal> getTerminal2();

    /**
     * Get the AC terminal at provided terminal number
     */
    const Terminal& getTerminal(const TerminalNumber& terminalNumber) const;
    /**
     * Get the AC terminal provided terminal number
     */
    Terminal& getTerminal(const TerminalNumber& terminalNumber);

    /**
     * Get the first DC Terminal
     */
    const DcTerminal& getDcTerminal1() const;
    /**
     * Get the first DC Terminal
     */
    DcTerminal& getDcTerminal1();

    /**
     * Get the second DC Terminal
     */
    const DcTerminal& getDcTerminal2() const;
    /**
     * Get the second DC Terminal
     */
    DcTerminal& getDcTerminal2();

    /**
     * Get the DC Terminal at provided terminal number
     */
    const DcTerminal& getDcTerminal(const TerminalNumber& terminalNumber) const;
    /**
     * Get the DC Terminal at provided terminal number
     */
    DcTerminal& getDcTerminal(const TerminalNumber& terminalNumber);

    /**
     * Get the terminal number the given AC terminal is connected to
     */
    TerminalNumber getTerminalNumber(const Terminal& terminal) const;
    /**
     * Get the terminal number the given DC terminal is connected to
     */
    TerminalNumber getTerminalNumber(const DcTerminal& terminal) const;

    /**
     * Get the idle loss (MW).
     */
    double getIdleLoss() const;
    /**
     * Get the switching loss (MW/A).
     */
    double getSwitchingLoss() const;
    /**
     * Get the resistive loss.
     */
    double getResistiveLoss() const;
    /**
     * Get the point of common coupling terminal, if pcc Terminal is not set return the first AC Terminal by default
     */
    const Terminal& getPccTerminal() const;
    /**
     * Get the point of common coupling terminal, if pcc Terminal is not set return the first AC Terminal by default
     */
    Terminal& getPccTerminal();
    /**
     * Get the control mode of the converter
     */
    const ControlMode& getControlMode() const;
    /**
     * Get the target active power at point of common coupling (MW)
     */
    double getTargetP() const;
    /**
     * Get the target DC voltage (kV DC)
     */
    double getTargetVdc() const;

    virtual void remove() override;

    /**
     * Set the idle loss (MW).
     */
    AcDcConverter& setIdleLoss(double idleLoss);
    /**
     * Set the switching loss (MW/A).
     */
    AcDcConverter& setSwitchingLoss(double switchingLoss);
    /**
     * Set the resistive loss.
     */
    AcDcConverter& setResistiveLoss(double resistiveLoss);
    /**
     * Set the point of common coupling terminal
     */
    AcDcConverter& setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal);
    /**
     * Set the control mode of the converter
     */
    AcDcConverter& setControlMode(const ControlMode& controlMode);
    /**
     * Set the target active power at point of common coupling (MW)
     */
    AcDcConverter& setTargetP(double targetP);
        /**
     * Set the target DC voltage (kV DC)
     */
    AcDcConverter& setTargetVdc(double targetVdc);

private:
    double m_idleLoss;
    double m_switchingLoss;
    double m_resistiveLoss;

    //Regulation: Managed through Referrer<Terminal>
    stdcxx::Reference<Terminal> m_pccRegulatingTerminal;
    std::vector<ControlMode> m_controlMode;

    std::vector<double> m_targetP;
    std::vector<double> m_targetVdc;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ACDCCONVERTER_HPP
