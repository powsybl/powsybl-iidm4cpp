/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORENTSOECATEGORYADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORENTSOECATEGORYADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class GeneratorEntsoeCategoryAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit GeneratorEntsoeCategoryAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~GeneratorEntsoeCategoryAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    GeneratorEntsoeCategoryAdder(const GeneratorEntsoeCategoryAdder&) = default;

    /**
     * Move constructor
     */
    GeneratorEntsoeCategoryAdder(GeneratorEntsoeCategoryAdder&&) = default;

    /**
     * Copy assignment operator
     */
    GeneratorEntsoeCategoryAdder& operator=(const GeneratorEntsoeCategoryAdder&) = delete;

    /**
     * Move assignment operator
     */
    GeneratorEntsoeCategoryAdder& operator=(GeneratorEntsoeCategoryAdder&&) = delete;

    /**
     * Set code
     *
     * @param code the code
     *
     * @return this GeneratorEntsoeCategoryAdder object
     */
    GeneratorEntsoeCategoryAdder& withCode(unsigned long code);

protected:
    /**
     * Creates the GeneratorEntsoeCategory extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    unsigned long m_code = 0UL;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORENTSOECATEGORYADDER_HPP
