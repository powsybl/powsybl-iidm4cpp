/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP

#include <functional>
#include <vector>

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

class ShuntCompensator : public Injection {
public:
    ShuntCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
                     double bPerSection, unsigned long maximumSectionCount, unsigned long currentSectionCount, Terminal& terminal,
                     bool voltageRegulatorOn, double targetV, double targetDeadband);

    ~ShuntCompensator() noexcept override = default;

    double getbPerSection() const;

    double getCurrentB() const;

    unsigned long getCurrentSectionCount() const;

    double getMaximumB() const;

    unsigned long getMaximumSectionCount() const;

    const Terminal& getRegulatingTerminal() const;

    Terminal& getRegulatingTerminal();

    double getTargetDeadband() const;

    double getTargetV() const;

    bool isVoltageRegulatorOn() const;

    ShuntCompensator& setbPerSection(double bPerSection);

    ShuntCompensator& setCurrentSectionCount(unsigned long currentSectionCount);

    ShuntCompensator& setMaximumSectionCount(unsigned long maximumSectionCount);

    ShuntCompensator& setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal);

    ShuntCompensator& setTargetDeadband(double targetDeadband);

    ShuntCompensator& setTargetV(double targetV);

    ShuntCompensator& setVoltageRegulatorOn(bool voltageRegulatorOn);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    double m_bPerSection;

    unsigned long m_maximumSectionCount;

    std::vector<unsigned long> m_currentSectionCount;

    std::reference_wrapper<Terminal> m_regulatingTerminal;

    std::vector<bool> m_voltageRegulatorOn;

    std::vector<double> m_targetV;

    std::vector<double> m_targetDeadband;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP
