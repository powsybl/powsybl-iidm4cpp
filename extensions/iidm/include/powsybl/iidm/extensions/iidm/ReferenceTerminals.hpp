/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCETERMINALS_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCETERMINALS_HPP

#include <vector>
#include <map>

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>
#include <powsybl/iidm/IdentifiableType.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Network;

namespace extensions {

namespace iidm {

class ReferenceTerminals : public AbstractMultiVariantIdentifiableExtension, public Referrer<Terminal> {
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
private:
    /**
     * if given variant terminals are not present in any other variants, unregister them
     */
    void unregisterReferencedTerminalIfNeeded(unsigned long variantIndex);
    /**
     * If given terminal is not already referenced by this extension, in any variant, register it.
     */
    void registerReferencedTerminalIfNeeded(Terminal& terminal);

public:
    ~ReferenceTerminals() noexcept override = default;

    ReferenceTerminals& setReferenceTerminals(const std::vector<stdcxx::Reference<Terminal>>& terminals);
    ReferenceTerminals& addReferenceTerminal(Terminal& terminalRef);

    std::vector<stdcxx::Reference<Terminal>>& getReferenceTerminals();

    stdcxx::const_range<Terminal> getReferenceTerminals() const;

    ReferenceTerminals& reset();

    /**
     * Gets the reference terminals defined in the given network.
     * This method returns only the terminal from the extensions attached to the given network,
     * in case of subnetworks, be careful whether you want the extension from the root network or a subnetwork
     * @param network network from which reference terminals should be listed
     */
    static std::vector<stdcxx::Reference<Terminal>> getTerminals(Network& network);
    /**
     * Defines/add a terminal as reference in the root network of the given terminal for the current variant
     * @param terminal terminal to be added as reference terminal
     */
    static void addTerminal(Terminal& terminal);
    /**
     * Deletes all defined reference terminals in the network and all its subnetworks for the current variant
     * @param network network whose reference terminals should be deleted
     */
    static void reset(Network& network);


protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    explicit ReferenceTerminals(Network& network, const std::vector<stdcxx::Reference<Terminal>>& terminals);
    friend class ReferenceTerminalsAdder;

    static void checkTerminalInNetwork(const Terminal& terminal, const Network& network);

private:

    //Referrer<Terminal> inheritance manage these references. Register each terminal only once (even if referenced by several variants)
    std::vector<std::vector<stdcxx::Reference<Terminal>>> m_referenceTerminals;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCETERMINALS_HPP
