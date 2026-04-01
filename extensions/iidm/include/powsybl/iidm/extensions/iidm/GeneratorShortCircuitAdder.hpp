/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSHORTCIRCUITADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSHORTCIRCUITADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuitAdder.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class GeneratorShortCircuitAdder : public AbstractShortCircuitAdder<GeneratorShortCircuitAdder> {
public:
    /**
     * Constructor
     */
    explicit GeneratorShortCircuitAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    GeneratorShortCircuitAdder(const GeneratorShortCircuitAdder&) = default;

    /**
     * Move constructor
     */
    GeneratorShortCircuitAdder(GeneratorShortCircuitAdder&&) = default;

    /**
     * Destructor
     */
    ~GeneratorShortCircuitAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    GeneratorShortCircuitAdder& operator=(const GeneratorShortCircuitAdder&) = delete;

    /**
     * Move assignment operator
     */
    GeneratorShortCircuitAdder& operator=(GeneratorShortCircuitAdder&&) = delete;

protected:
    /**
     * Creates the GeneratorShortCircuit extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSHORTCIRCUITADDER_HPP
