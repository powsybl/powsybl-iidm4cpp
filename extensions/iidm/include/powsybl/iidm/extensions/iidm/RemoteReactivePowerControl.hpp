/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROL_HPP

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>
#include <powsybl/iidm/Referrer.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Generator;
class Network;
class Terminal;

namespace extensions {

namespace iidm {

class RemoteReactivePowerControlAdder;

class RemoteReactivePowerControl : public AbstractMultiVariantIdentifiableExtension, public Referrer<Terminal> {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

    void cleanup() override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public: //Referrer<Terminal>
    void onReferencedRemoval(Terminal& removedReference) override;
    void onReferencedReplacement(Terminal& oldReference, Terminal& newReference) override;

public:
    ~RemoteReactivePowerControl() noexcept override = default;

    const Terminal& getRegulatingTerminal() const;

    Terminal& getRegulatingTerminal();

    double getTargetQ() const;

    bool isEnabled() const;

    RemoteReactivePowerControl& setTargetQ(double targetQ);
    RemoteReactivePowerControl& setEnabled(bool enabled);
    RemoteReactivePowerControl& setRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    RemoteReactivePowerControl(Generator& generator, double targetQ, Terminal& terminal, bool enabled);

    friend RemoteReactivePowerControlAdder;

    static double checkTargetQ(double targetQ);
    static void checkRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal, const Network& network);

private:
    std::vector<double> m_targetQ;

    std::reference_wrapper<Terminal> m_regulatingTerminal; //Managed through Referrer<Terminal> inheritance

    std::vector<bool> m_enabled;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROL_HPP
