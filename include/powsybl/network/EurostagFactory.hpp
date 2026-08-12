/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_EUROSTAGFACTORY_HPP
#define POWSYBL_IIDM_NETWORK_EUROSTAGFACTORY_HPP

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace network {

class EurostagFactory {
public:
    static iidm::Network createTutorial1Network();

    static iidm::Network createWithCurrentLimits();

    static iidm::Network createWithFixedLimits();

    static iidm::Network createWithVoltageAngleLimit();

    static iidm::Network createWithTieLinesAndAreas();

    /**
     * Selected limits:
     * On line 'NHV1_NHV2_1' :
     * Side 1 (also has a 'not_activated' group)
     * <pre>
     *      Default      activated_1_1       activated_1_2
     *      |                                ---- IT0.5 1600 (30s)
     *      |            ---- IT1 1500 (60s)
     *      |            ---- IT10 1200 (600s)
     *      |            ---- ITP 1100
     *      |                                ---- IT40 700 (2400s)
     *      | ---- ITP 500
     *      |                                ---- ITP 300
     * </pre>
     * Side 2
     * <pre>
     *      Default             activated_2_1
     *      |                   ---- IT10 1000 (600s)
     *      | ---- ITP 600      ---- ITP 600
     * </pre>
     *
     * On line 'NHV1_NHV2_2' :
     * Side 1
     * <pre>
     *      Default
     *      | ---- IT20 1200 (1200s)
     *      | ---- ITP 1100
     * </pre>
     * Side 2 (also has a 'not_activated' group)
     * <pre>
     *     Default              activated_2_1       activated_2_2
     *     |                    ---- IT20 600 (1200s)
     *     | ---- ITP 500
     *     |                                        ---- ITP 300
     *     |                    ---- ITP 200
     * </pre>
     */
    static iidm::Network createWithMultipleSelectedFixedCurrentLimits();

    /**
     * Selected limits:
     * On three-winding transformer 'NGEN_V2_NHV1', side 3 (also has a 'not_activated' group) :
     * <pre>
     *      Default      activated_3_1
     *      |            ---- IT45 400 (2700s)
     *      |            ---- ITP 350
     *      | ---- ITP 250
     * </pre>
     */
    static iidm::Network createWithMultipleSelectedFixedActivePowerLimits();

    static iidm::Network createWith3wTransformer();

    /**
     * Selected limits:
     * On two-winding transformer 'NGEN_NHV1', side 2 :
     * <pre>
     *      activated_2_1             activated_2_2
     *                                ---- IT20 250 (1200s)
     *      ---- IT10 240 (600s)      ---- ITP 240
     *      ---- ITP 230
     * </pre>
     */
    static iidm::Network createWithMultipleSelectedFixedApparentPowerLimits();

public:
    EurostagFactory() = delete;
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORK_EUROSTAGFACTORY_HPP
