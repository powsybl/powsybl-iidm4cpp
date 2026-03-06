/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLACKTERMINAL_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLACKTERMINAL_HPP

#include <vector>

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>
#include <powsybl/iidm/Referrer.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Network;
class Terminal;
class VoltageLevel;

namespace extensions {

class SlackTerminal : public AbstractMultiVariantIdentifiableExtension, public Referrer<Terminal> {
public:
    static void attach(Bus& bus);

    static void reset(Network& network);

    static void reset(VoltageLevel& voltageLevel, const stdcxx::Reference<Terminal>& terminal);

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

    void cleanup() override;

public: //Referrer<Terminal>
    void onReferencedRemoval(Terminal& removedReference) override;
private:
    /**
     * if given variant terminal is not present in any other variants, unregister it
     */
    void unregisterReferencedTerminalIfNeeded(unsigned long variantIndex);
    /**
     * If given terminal is not already referenced by this extension, in any variant, register it.
     */
    void registerReferencedTerminalIfNeeded(Terminal& terminal);

public:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    SlackTerminal(VoltageLevel& voltageLevel, Terminal& terminal);

    stdcxx::CReference<Terminal> getTerminal() const;

    stdcxx::Reference<Terminal> getTerminal();

    bool isEmpty() const;

    SlackTerminal& setTerminal(const stdcxx::Reference<Terminal>& terminal);

    SlackTerminal& setTerminal(const stdcxx::Reference<Terminal>& terminal, bool cleanIfEmpty);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    // Referrer<Terminal> inheritance manage theses references. Register a terminal only once (even if referenced by several variants)
    std::vector<stdcxx::Reference<Terminal>> m_terminals;
};

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLACKTERMINAL_HPP
