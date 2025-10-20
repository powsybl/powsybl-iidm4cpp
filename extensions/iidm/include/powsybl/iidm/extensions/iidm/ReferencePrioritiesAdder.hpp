/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITIESADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITIESADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ReferencePrioritiesAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ReferencePrioritiesAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    ReferencePrioritiesAdder(const ReferencePrioritiesAdder&) = default;

    /**
     * Move constructor
     */
    ReferencePrioritiesAdder(ReferencePrioritiesAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~ReferencePrioritiesAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    ReferencePrioritiesAdder& operator=(const ReferencePrioritiesAdder&) = delete;

    /**
     * Move assignment operator
     */
    ReferencePrioritiesAdder& operator=(ReferencePrioritiesAdder&&) = delete;

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITIESADDER_HPP
