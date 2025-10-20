/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCETERMINALSADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCETERMINALSADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <vector>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ReferenceTerminalsAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ReferenceTerminalsAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    ReferenceTerminalsAdder(const ReferenceTerminalsAdder&) = default;

    /**
     * Move constructor
     */
    ReferenceTerminalsAdder(ReferenceTerminalsAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~ReferenceTerminalsAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    ReferenceTerminalsAdder& operator=(const ReferenceTerminalsAdder&) = delete;

    /**
     * Move assignment operator
     */
    ReferenceTerminalsAdder& operator=(ReferenceTerminalsAdder&&) = delete;

    ReferenceTerminalsAdder& withTerminals(const std::vector<stdcxx::Reference<Terminal>>& terminals);

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

    std::vector<stdcxx::Reference<Terminal>> m_terminals;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCETERMINALSADDER_HPP
