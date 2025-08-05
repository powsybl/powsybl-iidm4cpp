/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSTARTUPADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSTARTUPADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class GeneratorStartupAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit GeneratorStartupAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~GeneratorStartupAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    GeneratorStartupAdder(const GeneratorStartupAdder&) = default;

    /**
     * Move constructor
     */
    GeneratorStartupAdder(GeneratorStartupAdder&&) = default;

    /**
     * Copy assignment operator
     */
    GeneratorStartupAdder& operator=(const GeneratorStartupAdder&) = delete;

    GeneratorStartupAdder& withForcedOutageRate(double forcedOutageRate);

    GeneratorStartupAdder& withStartupCost(double startupCost);

    GeneratorStartupAdder& withMarginalCost(double marginalCost);

    GeneratorStartupAdder& withPlannedOutageRate(double plannedOutageRate);

    GeneratorStartupAdder& withPredefinedActivePowerSetpoint(double predefinedActivePowerSetpoint);

protected:
    /**
     * Creates the GeneratorStartup extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    double m_predefinedActivePowerSetpoint = stdcxx::nan();

    double m_startupCost = stdcxx::nan();

    double m_marginalCost = stdcxx::nan();

    double m_plannedOutageRate = stdcxx::nan();

    double m_forcedOutageRate = stdcxx::nan();
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSTARTUPADDER_HPP
