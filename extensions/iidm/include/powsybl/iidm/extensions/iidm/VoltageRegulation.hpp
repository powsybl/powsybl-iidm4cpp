/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEREGULATION_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEREGULATION_HPP

#include <vector>

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>
#include <powsybl/iidm/Referrer.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Battery;

namespace extensions {

namespace iidm {

class VoltageRegulation : public AbstractMultiVariantIdentifiableExtension, public Referrer<Terminal> {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

    void cleanup() override;

public: // Referrer<Terminal>
    void onReferencedRemoval(Terminal& removedReference) override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    ~VoltageRegulation() noexcept override = default;

    stdcxx::CReference<Terminal> getRegulatingTerminal() const;

    double getTargetV() const;

    bool isVoltageRegulatorOn() const;

    VoltageRegulation& setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal);

    VoltageRegulation& setTargetV(double targetV);

    VoltageRegulation& setVoltageRegulatorOn(bool voltageRegulatorOn);


protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    explicit VoltageRegulation(Battery& battery, bool voltageRegulatorOn, double targetV);
    explicit VoltageRegulation(Battery& battery, const stdcxx::Reference<Terminal>& regulatingTerminal, bool voltageRegulatorOn, double targetV);
    friend class VoltageRegulationAdder;

    static void checkTerminalInNetwork(const stdcxx::Reference<Terminal>& regulatingTerminal, const Network& network);

    const Network& getNetworkFromExtendable() const;

private:
    std::vector<bool> m_voltageRegulatorOn;
    std::vector<double> m_targetV;

    stdcxx::Reference<Terminal> m_regulatingTerminal;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEREGULATION_HPP
