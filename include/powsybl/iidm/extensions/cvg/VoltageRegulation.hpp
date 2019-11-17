/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATION_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATION_HPP

#include <vector>

#include <powsybl/iidm/AbstractMultiVariantConnectableExtension.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

template <typename T>
class VoltageRegulation : public AbstractMultiVariantConnectableExtension {
public:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    VoltageRegulation(T& extendable, bool voltageRegulatorOn, double targetV);

    VoltageRegulation(T& extendable, const stdcxx::CReference<Terminal>& regulatingTerminal, bool voltageRegulatorOn, double targetV);

    stdcxx::CReference<Terminal> getRegulatingTerminal() const;

    double getTargetV() const;

    bool isVoltageRegulatorOn() const;

    VoltageRegulation<T>& setRegulatingTerminal(const stdcxx::CReference<Terminal>& regulatingTerminal);

    VoltageRegulation<T>& setTargetV(double targetV);

    VoltageRegulation<T>& setVoltageRegulatorOn(bool voltageRegulatorOn);

private:
    static void checkRegulatingTerminal(const stdcxx::CReference<Terminal>& regulatingTerminal, const Network& network);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    const Network& getNetworkFromExtendable() const;

private:
    std::vector<bool> m_voltageRegulatorOn;

    std::vector<double> m_targetV;

    stdcxx::CReference<Terminal> m_regulatingTerminal;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/extensions/cvg/VoltageRegulation.hxx>

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_VOLTAGEREGULATION_HPP

