/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_NETWORK_DCDETAILEDNETWORKFACTORY_HPP
#define POWSYBL_NETWORK_DCDETAILEDNETWORKFACTORY_HPP

#include <powsybl/iidm/Network.hpp>

#include <map>

namespace powsybl {

namespace network {

class DcDetailedNetworkFactory {

public: 
    static const char* const X_NODE_DC_1_FR;
    static const char* const X_NODE_DC_2_FR;
    static const char* const X_NODE_DC_1_GB;
    static const char* const X_NODE_DC_2_GB;
    static const char* const DC_NODE_FR_POS;
    static const char* const DC_NODE_FR_POS_A;
    static const char* const DC_NODE_FR_POS_B;
    static const char* const DC_NODE_FR_NEG;
    static const char* const DC_NODE_FR_NEG_A;
    static const char* const DC_NODE_FR_NEG_B;
    static const char* const DC_NODE_FR_MID;
    static const char* const DC_NODE_GB_POS;
    static const char* const DC_NODE_GB_POS_A;
    static const char* const DC_NODE_GB_POS_B;
    static const char* const DC_NODE_GB_NEG;
    static const char* const DC_NODE_GB_NEG_A;
    static const char* const DC_NODE_GB_NEG_B;
    static const char* const DC_NODE_GB_MID;
    static const char* const DC_GROUND_FR;
    static const char* const DC_GROUND_GB;
    static const char* const SUFFIX_NONE;
    static const char* const SUFFIX_1;
    static const char* const SUFFIX_2;
    static const char* const SUFFIX_400;
    static const char* const SUFFIX_400_I;
    static const char* const SUFFIX_150;
    static const char* const SUFFIX_150_1;
    static const char* const SUFFIX_150_2;
    static const char* const DC_NODE_POS_A1;
    static const char* const DC_NODE_POS_A2;
    static const char* const DC_NODE_POS_B1;
    static const char* const DC_NODE_POS_B2;
    static const char* const DC_NODE_NEG_A1;
    static const char* const DC_NODE_NEG_A2;
    static const char* const DC_NODE_NEG_B1;
    static const char* const DC_NODE_NEG_B2;
    static const char* const DC_LINE1;
    static const char* const DC_LINE2;

private:
    enum class Mode : unsigned char {
        ONE_T2WT,
        TWO_T2WT,
        T3WT
    };

public:
    static iidm::Network createLccMonopoleGroundReturn();
    static iidm::Network createLccMonopoleMetallicReturn();

    static iidm::Network createLccBipoleGroundReturn();
    static iidm::Network createLccBipoleGroundReturnNegativePoleOutage();
    /**
    * Each pole looks like this, two lines in parallel with multiple segments
    * to model e.g. overhead/underground/submarine portions.
    * Here showing only one pole:
    *
    *   FR                       2 ohm             4 ohm              2 ohm                      GB
    * converter -- switchA -- dcLine segment -- dcLine segment -- dcLine segment -- switchA -- converter
    *           \                                                                           /
    *            - switchB -- dcLine segment -- dcLine segment -- dcLine segment -- switchB
    *                            2 ohm             4 ohm              2 ohm
    *
    * when everything connected equivalent to one 5 ohm DcLine
     */
    static iidm::Network createLccBipoleGroundReturnWithDcLineSegments();

    static iidm::Network createVscSymmetricalMonopole();
    static iidm::Network createVscAsymmetricalMonopole();

    static std::string getVoltageLevelId(const iidm::Country& country, const std::string& xNode, const std::string& suffix);
    static std::string getBusId(const iidm::Country& country, const std::string& xNode, const std::string& suffix);
    static std::string getTransformerId(const iidm::Country& country, const std::string& xNode, const std::string& suffix);
    static std::string getLineId(const iidm::Country& country, const std::string& xNode, const std::string& suffix);



private:

    /**
     * Creates a simple one bus AC (sub)network with boundary lines.
     * <br/>
     * Example with FR and one xNode where FR exports 200 MW:
     * <pre>
     *     var net = createSimpleAcNetworkWithBoundaryLines(networkFactory, Country.FR, Map.of("xNode1", 200.));
     * </pre>
     *
     * <pre>
     *  targetP = 2000 MW / maxP = 4000 MW
     *  GEN-FR
     *    |
     *  (BUS-FR)-----(DLAC-FR-xNode1)  P0 = 200 MW
     *    |
     *  LOAD-FR
     *  P0 = 2000 MW - 200 MW = 1800 MW
     * </pre>
     */
    static iidm::Network& createSimpleAcNetworkWithBoundaryLines(iidm::Network& rootNetwork, const iidm::Country& country, const std::map<std::string, double>& xNodes);


    static std::string getId(const std::string& type, const iidm::Country& country, const std::string& xNode, const std::string& suffix);

    static void addDcAcElements(iidm::Network& network, const iidm::Country& country, const std::string& xNode, double exchange, const Mode& mode);

    static iidm::Network createLccMonopoleBase(const std::string& dcNetworkId);
    static iidm::Network createLccBipoleBase(const std::string& dcNetworkId);

    static iidm::Network createVscMonopoleBase(const std::string& dcNetworkId);


};


}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_NETWORK_DCDETAILEDNETWORKFACTORY_HPP
