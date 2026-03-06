/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/AreaBoundary.hpp>
#include <powsybl/iidm/AreaAdder.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>


namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(AreasTestSuite)

double tol = 1e-2;

BOOST_AUTO_TEST_CASE(AreasAttributes) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();

    Area& controlAreaA = network.getArea("ControlArea_A");
    Area& controlAreaB = network.getArea("ControlArea_B");
    Area& regionAB = network.getArea("Region_AB");

    BOOST_CHECK_EQUAL(IdentifiableType::AREA, controlAreaA.getType());
    BOOST_CHECK_EQUAL("ControlArea_A", controlAreaA.getId());
    BOOST_CHECK_EQUAL("Control Area A", controlAreaA.getNameOrId());
    BOOST_CHECK_EQUAL("ControlArea", controlAreaA.getAreaType());
    BOOST_CHECK_CLOSE(-602.6, controlAreaA.getInterchangeTarget(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getVoltageLevels()));
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getAreaBoundaries()));

    BOOST_CHECK_EQUAL("ControlArea_B", controlAreaB.getId());
    BOOST_CHECK_EQUAL("Control Area B", controlAreaB.getOptionalName());
    BOOST_CHECK_EQUAL("ControlArea", controlAreaB.getAreaType());
    BOOST_CHECK_CLOSE(+602.6, controlAreaB.getInterchangeTarget(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaB.getVoltageLevels()));
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaB.getAreaBoundaries()));

    BOOST_CHECK_EQUAL("Region_AB", regionAB.getId());
    BOOST_CHECK_EQUAL("Region AB", regionAB.getOptionalName());
    BOOST_CHECK_EQUAL("Region", regionAB.getAreaType());
    BOOST_CHECK(std::isnan(regionAB.getInterchangeTarget()));
    BOOST_CHECK_EQUAL(4, boost::size(regionAB.getVoltageLevels()));
    BOOST_CHECK_EQUAL(0, boost::size(regionAB.getAreaBoundaries()));

    controlAreaA.setInterchangeTarget(123.0);
    BOOST_CHECK_CLOSE(123.0, controlAreaA.getInterchangeTarget(), std::numeric_limits<double>::epsilon());
    controlAreaA.setInterchangeTarget(stdcxx::nan());
    BOOST_CHECK(std::isnan(controlAreaA.getInterchangeTarget()));
}

BOOST_AUTO_TEST_CASE(AreasMultiVariants) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");

    //Add Variants:
    VariantManager& variantManager = network.getVariantManager();
    std::string variant1 = "s1";
    std::string variant2 = "s2";
    std::string variant3 = "s3";
    std::string variant4 = "s4";
    variantManager.cloneVariant(VariantManager::getInitialVariantId(),{variant1, variant2, variant3, variant4});

    variantManager.setWorkingVariant(variant4);
    //initial value copy of the initial
    BOOST_CHECK_CLOSE(-602.6, controlAreaA.getInterchangeTarget(), std::numeric_limits<double>::epsilon());
    //set new value in variant 4
    controlAreaA.setInterchangeTarget(123.0);

    //remove Variant 2
    variantManager.removeVariant(variant2);
    //value in working variant unchanged
    BOOST_CHECK_CLOSE(123.0, controlAreaA.getInterchangeTarget(), std::numeric_limits<double>::epsilon());

    variantManager.cloneVariant(variant4, "s2b");
    variantManager.setWorkingVariant("s2b");
    //initial value copy of the initial
    BOOST_CHECK_CLOSE(123.0, controlAreaA.getInterchangeTarget(), std::numeric_limits<double>::epsilon());

    //verify initial variant unmodified
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(-602.6, controlAreaA.getInterchangeTarget(), std::numeric_limits<double>::epsilon());

    //remove working variant 4
    variantManager.setWorkingVariant(variant4);
    variantManager.removeVariant(variant4);
    POWSYBL_ASSERT_THROW(controlAreaA.getInterchangeTarget(), PowsyblException, "Variant index not set");

}

BOOST_AUTO_TEST_CASE(getAreaBoundaryTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");

    DanglingLine& dl = network.getDanglingLine("NHV1_XNODE1");

    std::shared_ptr<AreaBoundary> areaBoundary = controlAreaA.getAreaBoundary(dl.getBoundary());
    BOOST_CHECK(static_cast<bool>(areaBoundary));
    BOOST_CHECK(areaBoundary->isAc());
    BOOST_CHECK_EQUAL(controlAreaA.getId(), areaBoundary->getArea().getId());
    BOOST_CHECK(stdcxx::areSame(controlAreaA, areaBoundary->getArea()));
    BOOST_CHECK_CLOSE(-301.44, areaBoundary->getP(), tol);
    BOOST_CHECK_CLOSE(-116.55, areaBoundary->getQ(), tol);

    controlAreaA.removeAreaBoundary(dl.getBoundary());
    areaBoundary = controlAreaA.getAreaBoundary(dl.getBoundary());
    BOOST_CHECK(!static_cast<bool>(areaBoundary));

}

BOOST_AUTO_TEST_CASE(areaInterchangeTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    Area& controlAreaB = network.getArea("ControlArea_B");
    Area& regionAB = network.getArea("Region_AB");
    DanglingLine& dl = network.getDanglingLine("NHV1_XNODE1");

    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getAcInterchange(), tol);
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getDcInterchange());
    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getInterchange(), tol);

    BOOST_CHECK_CLOSE(+602.88, controlAreaB.getAcInterchange(), tol);
    BOOST_CHECK_SMALL(0.0, controlAreaB.getDcInterchange());
    BOOST_CHECK_CLOSE(+602.88, controlAreaB.getInterchange(), tol);

    // no boundaries defined
    BOOST_CHECK_EQUAL(0.0, regionAB.getAcInterchange());
    BOOST_CHECK_EQUAL(0.0, regionAB.getDcInterchange());
    BOOST_CHECK_EQUAL(0.0, regionAB.getInterchange());

    // verify NaN do not mess up the calculation
    dl.getTerminal().setP(stdcxx::nan());
    BOOST_CHECK_CLOSE(-301.44, controlAreaA.getAcInterchange(), tol);
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getDcInterchange());
    BOOST_CHECK_CLOSE(-301.44, controlAreaA.getInterchange(), tol);

}

BOOST_AUTO_TEST_CASE(getAreasTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();

    std::set<std::string> expectedAreasId = {"ControlArea_A", "ControlArea_B", "Region_AB"};
    std::set<std::string> expectedAreasType = {"ControlArea", "Region"};

    auto areas = network.getAreas();
    std::set<std::string> areaTypes = network.getAreaTypes();


    BOOST_CHECK_EQUAL(3, boost::size(areas));
    BOOST_CHECK_EQUAL(3, network.getAreaCount());
    BOOST_CHECK_EQUAL(2, areaTypes.size());
    BOOST_CHECK_EQUAL(2, network.getAreaTypeCount());

    for (auto& area : areas) {
        BOOST_CHECK(expectedAreasId.find(area.getId())!=expectedAreasId.end());
    }
    for (auto& areaId : expectedAreasId) {
        BOOST_CHECK_NO_THROW(network.getArea(areaId));
    }
    for (auto& areaType : areaTypes) {
        BOOST_CHECK(expectedAreasType.find(areaType)!=expectedAreasType.end());
    }
}

BOOST_AUTO_TEST_CASE(addVoltageLevelTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    Area& regionAB = network.getArea("Region_AB");
    
    auto& newVl = network.getVoltageLevel("VLHV1").getSubstation().get().newVoltageLevel()
                .setId("NewVl")
                .setNominalV(400.0)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();

    controlAreaA.addVoltageLevel(newVl);
    newVl.addArea(regionAB);

    std::set<std::string> expectedAreasNewVl = {"ControlArea_A", "Region_AB"};
    std::set<std::string> expectedVlAreaA = {"VLGEN", "VLHV1", "NewVl"};
    std::set<std::string> expectedVlRegionAB = {"VLGEN", "VLHV1", "NewVl", "VLHV2", "VLLOAD"};

    BOOST_CHECK_EQUAL(2, boost::size(newVl.getAreas()));
    BOOST_CHECK_EQUAL(3, boost::size(controlAreaA.getVoltageLevels()));
    BOOST_CHECK_EQUAL(5, boost::size(regionAB.getVoltageLevels()));
    for (auto& area : newVl.getAreas()) {
        BOOST_CHECK(expectedAreasNewVl.find(area.getId())!=expectedAreasNewVl.end());
    }
    for (auto& vl : controlAreaA.getVoltageLevels()) {
        BOOST_CHECK(expectedVlAreaA.find(vl.getId())!=expectedVlAreaA.end());
    }
    for (auto& vl : regionAB.getVoltageLevels()) {
        BOOST_CHECK(expectedVlRegionAB.find(vl.getId())!=expectedVlRegionAB.end());
    }

    //add already present vl in area, or area in voltageLevel
    controlAreaA.addVoltageLevel(network.getVoltageLevel("VLHV1"));
    network.getVoltageLevel("VLHV1").addArea(controlAreaA);

    BOOST_CHECK_EQUAL(2, boost::size(network.getVoltageLevel("VLHV1").getAreas()));
    BOOST_CHECK_EQUAL(3, boost::size(controlAreaA.getVoltageLevels()));

}

BOOST_AUTO_TEST_CASE(areaWithTerminalTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    auto& twt = network.getTwoWindingsTransformer("NGEN_NHV1");
    Area& controlAreaA = network.getArea("ControlArea_A");
    Area& controlAreaB = network.getArea("ControlArea_B");
    DanglingLine& dlXnode1A = network.getDanglingLine("NHV1_XNODE1");
    DanglingLine& dlXnode1B = network.getDanglingLine("XNODE1_NHV2");
    DanglingLine& dlXnode2A = network.getDanglingLine("NHV1_XNODE2");
    DanglingLine& dlXnode2B = network.getDanglingLine("XNODE2_NHV2");

    controlAreaA.removeAreaBoundary(dlXnode1A.getBoundary())
                .removeAreaBoundary(dlXnode2A.getBoundary());
    controlAreaB.removeAreaBoundary(dlXnode1B.getBoundary())
                .removeAreaBoundary(dlXnode2B.getBoundary());

    BOOST_CHECK(!static_cast<bool>(controlAreaA.getAreaBoundary(twt.getTerminal2())));

    controlAreaA.newAreaBoundary().setTerminal(twt.getTerminal2()).setAc(true).add();
    controlAreaB.newAreaBoundary().setTerminal(dlXnode1A.getTerminal()).setAc(true).add()
                .newAreaBoundary().setTerminal(dlXnode2A.getTerminal()).setAc(true).add();

    std::shared_ptr<AreaBoundary> areaBoundary = controlAreaA.getAreaBoundary(twt.getTerminal2());
    BOOST_CHECK(static_cast<bool>(areaBoundary));

    BOOST_CHECK_EQUAL(controlAreaA.getId(), areaBoundary->getArea().getId());
    BOOST_CHECK_CLOSE(-604.89, areaBoundary->getP(), tol);
    BOOST_CHECK_CLOSE(-197.48, areaBoundary->getQ(), tol);

    BOOST_CHECK_CLOSE(-604.89, controlAreaA.getAcInterchange(), tol);
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getDcInterchange());
    BOOST_CHECK_CLOSE(-604.89, controlAreaA.getInterchange(), tol);

    BOOST_CHECK_CLOSE(+604.89, controlAreaB.getAcInterchange(), tol);
    BOOST_CHECK_EQUAL(0.0, controlAreaB.getDcInterchange());
    BOOST_CHECK_CLOSE(+604.89, controlAreaB.getInterchange(), tol);

    // verify NaN do not mess up the calculation
    twt.getTerminal2().setP(stdcxx::nan());
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getAcInterchange());
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getDcInterchange());
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getInterchange());

    //remove terminal boundaries
    controlAreaB.removeAreaBoundary(dlXnode1A.getTerminal())
                .removeAreaBoundary(dlXnode2A.getTerminal());
    BOOST_CHECK_EQUAL(0, boost::size(controlAreaB.getAreaBoundaries()));

}

BOOST_AUTO_TEST_CASE(addSameBoundaryTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    DanglingLine& dlXnode1A = network.getDanglingLine("NHV1_XNODE1");
    DanglingLine& dlXnode2A = network.getDanglingLine("NHV1_XNODE2");
    
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getAreaBoundaries()));
    // re-add
    controlAreaA.newAreaBoundary().setBoundary(dlXnode1A.getBoundary()).setAc(true).add()
                .newAreaBoundary().setBoundary(dlXnode2A.getBoundary()).setAc(true).add();
    // no change
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getAreaBoundaries()));
    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getAcInterchange(), tol);
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getDcInterchange());
    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getInterchange(), tol);

    // change them to DC
    controlAreaA.newAreaBoundary().setBoundary(dlXnode1A.getBoundary()).setAc(false).add()
                .newAreaBoundary().setBoundary(dlXnode2A.getBoundary()).setAc(false).add();
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getAreaBoundaries()));
    BOOST_CHECK_EQUAL(0.0, controlAreaA.getAcInterchange());
    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getDcInterchange(), tol);
    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getInterchange(), tol);
}

BOOST_AUTO_TEST_CASE(withDcTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    DanglingLine& dlXnode2A = network.getDanglingLine("NHV1_XNODE2");

    controlAreaA.newAreaBoundary().setBoundary(dlXnode2A.getBoundary()).setAc(false).add();
    BOOST_CHECK_CLOSE(-301.44, controlAreaA.getAcInterchange(), tol);
    BOOST_CHECK_CLOSE(-301.44, controlAreaA.getDcInterchange(), tol);
    BOOST_CHECK_CLOSE(-602.88, controlAreaA.getInterchange(), tol);
}

BOOST_AUTO_TEST_CASE(removeVoltageLevelTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getVoltageLevels()));
        
    controlAreaA.removeVoltageLevel(network.getVoltageLevel("VLHV1"))
                .removeVoltageLevel(network.getVoltageLevel("VLGEN"));
    BOOST_CHECK_EQUAL(0, boost::size(controlAreaA.getVoltageLevels()));
}

BOOST_AUTO_TEST_CASE(removeAreaBoundariesTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    DanglingLine& dlXnode1A = network.getDanglingLine("NHV1_XNODE1");
    DanglingLine& dlXnode2A = network.getDanglingLine("NHV1_XNODE2");
    BOOST_CHECK_EQUAL(2, boost::size(controlAreaA.getAreaBoundaries()));
        
    controlAreaA.removeAreaBoundary(dlXnode1A.getBoundary())
                .removeAreaBoundary(dlXnode2A.getBoundary());
    BOOST_CHECK_EQUAL(0, boost::size(controlAreaA.getAreaBoundaries()));
}

BOOST_AUTO_TEST_CASE(throwAddNewAreaSameTypeTest) {

    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    Area& controlAreaB = network.getArea("ControlArea_B");

    POWSYBL_ASSERT_THROW(controlAreaA.addVoltageLevel(network.getVoltageLevel("VLHV2")), PowsyblException, "VoltageLevel VLHV2 is already in Area of the same type=ControlArea with id=ControlArea_B");
    POWSYBL_ASSERT_THROW(network.getVoltageLevel("VLHV1").addArea(controlAreaB), PowsyblException, "VoltageLevel VLHV1 is already in Area of the same type=ControlArea with id=ControlArea_A");
}

BOOST_AUTO_TEST_CASE(throwAddVoltageLevelOtherNetwork) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Network& subnetwork = network.newSubnetwork("subnetwork_id", "code");
    VoltageLevel& newVoltageLevel = subnetwork.newVoltageLevel()
                .setId("newVoltageLevel")
                .setNominalV(400.0)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();
    Area& controlAreaA = network.getArea("ControlArea_A");

    POWSYBL_ASSERT_THROW(controlAreaA.addVoltageLevel(newVoltageLevel), PowsyblException, "VoltageLevel newVoltageLevel cannot be added to Area ControlArea_A. It does not belong to the same network or subnetwork.");

}

BOOST_AUTO_TEST_CASE(throwBoundaryOtherNetworkTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");

    Network& subnetwork = network.newSubnetwork("subnetwork_id", "code");
    VoltageLevel& sn1VL1 = subnetwork.newVoltageLevel()
                .setId("sub1_vl1")
                .setNominalV(400.0)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();
    Bus& bus = sn1VL1.getBusBreakerView().newBus()
                .setId("sub1_bus")
                .add();
    DanglingLine& danglingLine = sn1VL1.newDanglingLine()
                .setId("sub1_dl")
                .setP0(0.0)
                .setQ0(0.0)
                .setR(1.0)
                .setX(1.0)
                .setG(0.0)
                .setB(0.0)
                .setBus(bus.getId())
                .setPairingKey("XNODE")
                .add();
    AreaBoundaryAdder areaBoundaryAdder = controlAreaA.newAreaBoundary().setBoundary(danglingLine.getBoundary()).setAc(true);
    POWSYBL_ASSERT_THROW(areaBoundaryAdder.add(), PowsyblException, "Boundary of DanglingLine sub1_dl cannot be added to Area ControlArea_A boundaries. It does not belong to the same network or subnetwork.");

}

BOOST_AUTO_TEST_CASE(throwBoundaryAttributeNotSetTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");
    DanglingLine& dlXnode1A = network.getDanglingLine("NHV1_XNODE1");

    AreaBoundaryAdder areaBoundaryAdder1 = controlAreaA.newAreaBoundary().setAc(true);
    POWSYBL_ASSERT_THROW(areaBoundaryAdder1.add(), PowsyblException, "No AreaBoundary element (terminal or boundary) is set.");

    AreaBoundaryAdder areaBoundaryAdder2 = controlAreaA.newAreaBoundary().setBoundary(dlXnode1A.getBoundary());
    POWSYBL_ASSERT_THROW(areaBoundaryAdder2.add(), PowsyblException, "AreaBoundary AC flag is not set.");
}

BOOST_AUTO_TEST_CASE(removeAreaTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    Area& controlAreaA = network.getArea("ControlArea_A");

    auto voltageLevels = controlAreaA.getVoltageLevels();
    std::vector<stdcxx::Reference<VoltageLevel>> vls;
    for (auto& vl : voltageLevels) {
        vls.push_back(stdcxx::ref(vl));
    }

    BOOST_CHECK_EQUAL(1, network.getDanglingLine("NHV1_XNODE1").getBoundary().getReferrers().size());
    controlAreaA.remove();
    BOOST_CHECK_EQUAL(0, network.getDanglingLine("NHV1_XNODE1").getBoundary().getReferrers().size());

    POWSYBL_ASSERT_THROW(network.getArea("ControlArea_A"), PowsyblException, "Unable to find to the identifiable 'ControlArea_A'");
    for(auto& area : network.getAreas()) {
        BOOST_CHECK(area.getId()!="ControlArea_A");
    }
    for(auto& vl : vls) {
        POWSYBL_ASSERT_REF_FALSE(vl.get().getArea("ControlArea"));
        for(auto& area : vl.get().getAreas()) {
            BOOST_CHECK(area.getId()!="ControlArea_A");
        }
    }

    //Add a second Area on the same dangling line boundary
    network.newArea()
                .setId("ControlArea_B_Bis")
                .setName("Control Area B_Bis")
                .setAreaType("ControlAreaBis")
                .setInterchangeTarget(+602.6)
                .addVoltageLevel(network.getVoltageLevel("VLHV2"))
                .addVoltageLevel(network.getVoltageLevel("VLLOAD"))
                .addAreaBoundary(network.getDanglingLine("XNODE1_NHV2").getBoundary(), true)
                .addAreaBoundary(network.getDanglingLine("XNODE2_NHV2").getBoundary(), true)
                .add();
    Area& controlAreaB = network.getArea("ControlArea_B");
    Area& controlAreaBBis = network.getArea("ControlArea_B_Bis");

    controlAreaBBis.removeAreaBoundary(network.getDanglingLine("XNODE1_NHV2").getBoundary());

    BOOST_CHECK_EQUAL(2, network.getDanglingLine("XNODE2_NHV2").getBoundary().getReferrers().size());
    BOOST_CHECK_EQUAL(2, controlAreaB.getAreaBoundaries().size());
    BOOST_CHECK_EQUAL(1, controlAreaBBis.getAreaBoundaries().size());
    //Remove Dangling Line Boundary will remove all area boundaries attached to it:
    network.getTieLine("NHV1_NHV2_2").remove();
    network.getDanglingLine("XNODE2_NHV2").remove();
    BOOST_CHECK_EQUAL(1, controlAreaB.getAreaBoundaries().size());
    BOOST_CHECK_EQUAL(0, controlAreaBBis.getAreaBoundaries().size());
}



BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
