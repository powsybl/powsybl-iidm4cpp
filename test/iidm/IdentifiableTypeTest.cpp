/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/IdentifiableType.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(IdentifiableTypeTestSuite)

BOOST_AUTO_TEST_CASE(test) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    
    BOOST_CHECK_EQUAL(IdentifiableType::NETWORK, network.getType());

    BOOST_CHECK_EQUAL(IdentifiableType::SUBSTATION, network.getSubstation("S1").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::VOLTAGE_LEVEL, network.getVoltageLevel("S1VL1").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::HVDC_LINE, network.getHvdcLine("HVDC1").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::SWITCH, network.getSwitch("S1VL1_BBS_LD1_DISCONNECTOR").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::BUSBAR_SECTION, network.getBusbarSection("S1VL1_BBS").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::LINE, network.getLine("LINE_S2S3").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::TWO_WINDINGS_TRANSFORMER, network.getTwoWindingsTransformer("TWT").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::GENERATOR, network.getGenerator("GH1").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::LOAD, network.getLoad("LD1").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::SHUNT_COMPENSATOR, network.getShuntCompensator("SHUNT").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::STATIC_VAR_COMPENSATOR, network.getStaticVarCompensator("SVC").getType());

    BOOST_CHECK_EQUAL(IdentifiableType::HVDC_CONVERTER_STATION, network.getHvdcConverterStation("VSC1").getType());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
