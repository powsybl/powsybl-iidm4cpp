/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROLADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROLADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class VoltagePerReactivePowerControlAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit VoltagePerReactivePowerControlAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    VoltagePerReactivePowerControlAdder(const VoltagePerReactivePowerControlAdder&) = default;

    /**
     * Move constructor
     */
    VoltagePerReactivePowerControlAdder(VoltagePerReactivePowerControlAdder&&) = default;

    /**
     * Destructor
     */
    ~VoltagePerReactivePowerControlAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    VoltagePerReactivePowerControlAdder& operator=(const VoltagePerReactivePowerControlAdder&) = delete;

    /**
     * Move assignment operator
     */
    VoltagePerReactivePowerControlAdder& operator=(VoltagePerReactivePowerControlAdder&&) = delete;

    /**
     * Set slope value
     *
     * @param slope The slope value
     *
     * @return this VoltagePerReactivePowerControlAdder object
     */
    VoltagePerReactivePowerControlAdder& withSlope(double slope);

protected:  // ExtensionAdder
    /**
     * Creates the VoltagePerReactivePowerControl extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    double m_slope = 0.0;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROLADDER_HPP
