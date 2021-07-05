/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCOPERATORACTIVEPOWERRANGEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCOPERATORACTIVEPOWERRANGEADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class HvdcOperatorActivePowerRangeAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit HvdcOperatorActivePowerRangeAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    HvdcOperatorActivePowerRangeAdder(const HvdcOperatorActivePowerRangeAdder&) = default;

    /**
     * Move constructor
     */
    HvdcOperatorActivePowerRangeAdder(HvdcOperatorActivePowerRangeAdder&&) = default;

    /**
     * Destructor
     */
    ~HvdcOperatorActivePowerRangeAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    HvdcOperatorActivePowerRangeAdder& operator=(const HvdcOperatorActivePowerRangeAdder&) = delete;

    /**
     * Move assignment operator
     */
    HvdcOperatorActivePowerRangeAdder& operator=(HvdcOperatorActivePowerRangeAdder&&) = delete;

    /**
     * Set operator active power range from the converter station 1 to the converter station 2
     *
     * @param oprFromCS1toCS2 the operator active power range from the converter station 1 to the converter station 2 in MW
     *
     * @return this HvdcOperatorActivePowerRangeAdder object
     */
    HvdcOperatorActivePowerRangeAdder& withOprFromCS1toCS2(double oprFromCS1toCS2);

    /**
     * Set operator active power range from the converter station 2 to the converter station 1
     *
     * @param oprFromCS2toCS1 the operator active power range from the converter station 2 to the converter station 1 in MW
     *
     * @return this HvdcOperatorActivePowerRangeAdder object
     */
    HvdcOperatorActivePowerRangeAdder& withOprFromCS2toCS1(double oprFromCS2toCS1);

protected:
    /**
     * Creates the HvdcOperatorActivePowerRange extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    double m_oprFromCS1toCS2 = 0.0;

    double m_oprFromCS2toCS1 = 0.0;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCOPERATORACTIVEPOWERRANGEADDER_HPP
