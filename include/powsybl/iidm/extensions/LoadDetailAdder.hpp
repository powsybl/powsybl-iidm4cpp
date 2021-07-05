/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_LOADDETAILADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_LOADDETAILADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

class LoadDetailAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit LoadDetailAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    LoadDetailAdder(const LoadDetailAdder&) = default;

    /**
     * Move constructor
     */
    LoadDetailAdder(LoadDetailAdder&&) = default;

    /**
     * Destructor
     */
    ~LoadDetailAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    LoadDetailAdder& operator=(const LoadDetailAdder&) = delete;

    /**
     * Move assignment operator
     */
    LoadDetailAdder& operator=(LoadDetailAdder&&) = delete;

    /**
     * Set fixedActivePower value
     *
     * @param fixedActivePower the fixedActivePower value
     *
     * @return this LoadDetailAdder object
     */
    LoadDetailAdder& withFixedActivePower(double fixedActivePower);

    /**
     * Set fixedReactivePower value
     *
     * @param fixedReactivePower the fixedReactivePower value
     *
     * @return this LoadDetailAdder object
     */
    LoadDetailAdder& withFixedReactivePower(double fixedReactivePower);

    /**
     * Set variableActivePower value
     *
     * @param variableActivePower the variableActivePower value
     *
     * @return this LoadDetailAdder object
     */
    LoadDetailAdder& withVariableActivePower(double variableActivePower);

    /**
     * Set variableReactivePower value
     *
     * @param variableReactivePower the variableReactivePower value
     *
     * @return this LoadDetailAdder object
     */
    LoadDetailAdder& withVariableReactivePower(double variableReactivePower);

protected:
    /**
     * Creates the LoadDetail extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    double m_fixedActivePower = 0.0;

    double m_fixedReactivePower = 0.0;

    double m_variableActivePower = 0.0;

    double m_variableReactivePower = 0.0;
};

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_LOADDETAILADDER_HPP
