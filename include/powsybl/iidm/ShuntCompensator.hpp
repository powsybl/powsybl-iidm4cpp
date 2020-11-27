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
#include <powsybl/iidm/ShuntCompensatorModel.hpp>
#include <powsybl/iidm/ShuntCompensatorModelType.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

class ShuntCompensator : public Injection {
public:
    ShuntCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, std::unique_ptr<ShuntCompensatorModel>&& model,
                     unsigned long currentSectionCount, Terminal& terminal, bool voltageRegulatorOn, double targetV, double targetDeadband);

    ~ShuntCompensator() noexcept override = default;

    double getB() const;

    double getB(unsigned long sectionCount) const;

    double getG() const;

    double getG(unsigned long sectionCount) const;

    unsigned long getMaximumSectionCount() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<ShuntCompensatorModel, T>::value>::type>
    const T& getModel() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<ShuntCompensatorModel, T>::value>::type>
    T& getModel();

    const ShuntCompensatorModel& getModel() const;

    ShuntCompensatorModel& getModel();

    const ShuntCompensatorModelType& getModelType() const;

    const Terminal& getRegulatingTerminal() const;

    Terminal& getRegulatingTerminal();

    unsigned long getSectionCount() const;

    double getTargetDeadband() const;

    double getTargetV() const;

    bool isVoltageRegulatorOn() const;

    ShuntCompensator& setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal);

    ShuntCompensator& setSectionCount(unsigned long sectionCount);

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
    std::unique_ptr<ShuntCompensatorModel>& attach(std::unique_ptr<ShuntCompensatorModel>& model);

private:
    std::unique_ptr<ShuntCompensatorModel> m_model;

    /* the current number of section switched on */
    std::vector<unsigned long> m_sectionCount;

    std::reference_wrapper<Terminal> m_regulatingTerminal;

    std::vector<bool> m_voltageRegulatorOn;

    std::vector<double> m_targetV;

    std::vector<double> m_targetDeadband;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ShuntCompensator.hxx>

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATOR_HPP
