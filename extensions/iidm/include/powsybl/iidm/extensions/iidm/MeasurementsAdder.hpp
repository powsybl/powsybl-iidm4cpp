/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTSADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTSADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class MeasurementsAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit MeasurementsAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    MeasurementsAdder(const MeasurementsAdder&) = default;

    /**
     * Move constructor
     */
    MeasurementsAdder(MeasurementsAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~MeasurementsAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    MeasurementsAdder& operator=(const MeasurementsAdder&) = delete;

    /**
     * Move assignment operator
     */
    MeasurementsAdder& operator=(MeasurementsAdder&&) = delete;

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTSADDER_HPP
