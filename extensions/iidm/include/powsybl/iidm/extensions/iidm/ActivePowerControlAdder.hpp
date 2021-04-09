/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ActivePowerControlAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ActivePowerControlAdder(Extendable& extendable);


    /**
     * Copy constructor
     */
    ActivePowerControlAdder(const ActivePowerControlAdder&) = default;

    /**
     * Move constructor
     */
    ActivePowerControlAdder(ActivePowerControlAdder&&) = default;

    /**
     * Destructor
     */
    ~ActivePowerControlAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    ActivePowerControlAdder& operator=(const ActivePowerControlAdder&) = delete;

    /**
     * Move assignment operator
     */
    ActivePowerControlAdder& operator=(ActivePowerControlAdder&&) = delete;

    /**
     * Set droop value
     *
     * @param droop The droop value
     *
     * @return this ActivePowerControlAdder object
     */
    ActivePowerControlAdder& withDroop(double droop);

    /**
     * Set participate
     *
     * @param participate The participate status
     *
     * @return this ActivePowerControlAdder object
     */
    ActivePowerControlAdder& withParticipate(bool participate);

protected:
    /**
     * Creates the ActivePowerControl extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    bool m_participate = false;

    double m_droop = 0.0;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLADDER_HPP
