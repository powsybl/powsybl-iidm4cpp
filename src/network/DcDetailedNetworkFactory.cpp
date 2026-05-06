/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/network/DcDetailedNetworkFactory.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/Country.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcGroundAdder.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcLineAdder.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageSourceConverterAdder.hpp>

namespace powsybl {

namespace network {

const char* const DcDetailedNetworkFactory::X_NODE_DC_1_FR = "xNodeDc1fr";
const char* const DcDetailedNetworkFactory::X_NODE_DC_1_GB = "xNodeDc1gb";
const char* const DcDetailedNetworkFactory::DC_NODE_FR_POS = "dcNodeFrPos";
const char* const DcDetailedNetworkFactory::DC_NODE_FR_NEG = "dcNodeFrNeg";
const char* const DcDetailedNetworkFactory::DC_NODE_GB_POS = "dcNodeGbPos";
const char* const DcDetailedNetworkFactory::DC_NODE_GB_NEG = "dcNodeGbNeg";
const char* const DcDetailedNetworkFactory::DC_GROUND_FR = "dcGroundFr";
const char* const DcDetailedNetworkFactory::DC_GROUND_GB = "dcGroundGb";
const char* const DcDetailedNetworkFactory::SUFFIX_NONE = "";
const char* const DcDetailedNetworkFactory::SUFFIX_1 = "-1";
const char* const DcDetailedNetworkFactory::SUFFIX_2 = "-2";
const char* const DcDetailedNetworkFactory::SUFFIX_400 = "-400";
const char* const DcDetailedNetworkFactory::SUFFIX_400_I = "-400-I";
const char* const DcDetailedNetworkFactory::SUFFIX_150 = "-150";
const char* const DcDetailedNetworkFactory::SUFFIX_150_1 = "-150-1";
const char* const DcDetailedNetworkFactory::SUFFIX_150_2 = "-150-2";

iidm::Network DcDetailedNetworkFactory::createLccMonopoleGroundReturn() {
    return createLccMonopoleBase("LccMonopoleGroundReturn");
}
iidm::Network DcDetailedNetworkFactory::createLccMonopoleMetallicReturn() {
    iidm::Network network = createLccMonopoleBase("LccMonopoleMetallicReturn");
    network.getDcGround(DC_GROUND_GB).getDcTerminal().setConnected(false);
    network.getSubNetwork("LccMonopoleMetallicReturn").get()
                .newDcLine()
                .setId("dcLine2")
                .setDcNode1(DC_NODE_FR_NEG)
                .setConnected1(true)
                .setDcNode2(DC_NODE_GB_NEG)
                .setConnected2(true)
                .setR(5.0)
                .add();
    return network;
}

iidm::Network DcDetailedNetworkFactory::createVscSymmetricalMonopole() {
    return createVscMonopoleBase("VscSymmetricalMonopole");
}
iidm::Network DcDetailedNetworkFactory::createVscAsymmetricalMonopole() {
    iidm::Network network = createVscMonopoleBase("VscAsymmetricalMonopole");
    iidm::Network& dcNetwork = network.getSubNetwork("VscAsymmetricalMonopole").get();
    dcNetwork.getDcLine("dcLineNeg").remove();
    dcNetwork.getDcNode(DC_NODE_FR_POS).setNominalV(500.);
    dcNetwork.getDcNode(DC_NODE_GB_POS).setNominalV(500.);
    dcNetwork.getDcNode(DC_NODE_FR_NEG).setNominalV(1.);
    dcNetwork.getDcNode(DC_NODE_GB_NEG).setNominalV(1.);
    dcNetwork.newDcGround()
                .setId(DC_GROUND_FR)
                .setDcNode(DC_NODE_FR_NEG)
                .setConnected(true)
                .setR(0.0)
                .add();
    dcNetwork.newDcGround()
                .setId(DC_GROUND_GB)
                .setDcNode(DC_NODE_GB_NEG)
                .setConnected(true)
                .setR(0.0)
                .add();
    return network;
}

std::string DcDetailedNetworkFactory::getVoltageLevelId(const iidm::Country& country, const std::string& xNode, const std::string& suffix) {
    return getId("VLDC-", country, xNode, suffix);
}
std::string DcDetailedNetworkFactory::getBusId(const iidm::Country& country, const std::string& xNode, const std::string& suffix) {
    return getId("BUSDC-", country, xNode, suffix);
}
std::string DcDetailedNetworkFactory::getTransformerId(const iidm::Country& country, const std::string& xNode, const std::string& suffix) {
    return getId("TRDC-", country, xNode, suffix);
}
std::string DcDetailedNetworkFactory::getLineId(const iidm::Country& country, const std::string& xNode, const std::string& suffix) {
    return getId("LINEDC-", country, xNode, suffix);
}

iidm::Network& DcDetailedNetworkFactory::createSimpleAcNetworkWithDanglingLines(iidm::Network& rootNetwork, const iidm::Country& country, const std::map<std::string, double>& xNodes) {

    iidm::Network& acNetwork = rootNetwork.newSubnetwork(iidm::getCountryName(country), "test");
    iidm::Substation& s = acNetwork.newSubstation()
                .setId("S-" + iidm::getCountryName(country))
                .setCountry(country)
                .add();
    iidm::VoltageLevel& vl = s.newVoltageLevel()
                .setId("VL-" + iidm::getCountryName(country))
                .setNominalV(400.0)
                .setLowVoltageLimit(380.0)
                .setHighVoltageLimit(420.0)
                .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
                .add();
    iidm::Bus& b = vl.getBusBreakerView().newBus()
                .setId("BUS-" + iidm::getCountryName(country))
                .add();
    vl.newGenerator()
                .setId("GEN-" + iidm::getCountryName(country))
                .setMinP(0.0)
                .setMaxP(4000.0)
                .setVoltageRegulatorOn(true)
                .setTargetV(400.0)
                .setTargetP(2000.0)
                .setTargetQ(0.0)
                .setBus(b.getId())
                .add();
    iidm::Load& load = vl.newLoad()
                .setId("LOAD-" + iidm::getCountryName(country))
                .setP0(2000.0)
                .setQ0(0.0)
                .setBus(b.getId())
                .add();

    for(auto& xNode : xNodes) {
        load.setP0(load.getP0() - xNode.second);
        vl.newDanglingLine()
            .setId(getId("DLAC-", country, xNode.first, SUFFIX_NONE))
            .setBus(b.getId())
            .setR(0.3)
            .setX(3.0)
            .setB(0.0)
            .setG(0.0)
            .setP0(xNode.second)
            .setQ0(0.0)
            .setPairingKey(xNode.first)
            .add();
    }
    return acNetwork;
}


std::string DcDetailedNetworkFactory::getId(const std::string& type, const iidm::Country& country, const std::string& xNode, const std::string& suffix) {
    return type + iidm::getCountryName(country) + "-" + xNode + suffix;
}

void DcDetailedNetworkFactory::addDcAcElements(iidm::Network& network, const iidm::Country& country, const std::string& xNode, double exchange, const Mode& mode) {

    iidm::Substation& s = network.newSubstation()
                .setId(getId("SDC-", country, xNode, SUFFIX_NONE))
                .add();
    iidm::VoltageLevel& vldc400 = s.newVoltageLevel()
                .setId(getVoltageLevelId(country, xNode, SUFFIX_400))
                .setNominalV(400.0)
                .setLowVoltageLimit(380.0)
                .setHighVoltageLimit(420.0)
                .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
                .add();
    iidm::Bus& bDc400 = vldc400.getBusBreakerView().newBus()
                .setId(getBusId(country, xNode, SUFFIX_400))
                .add();
    vldc400.newDanglingLine()
                .setId(getId("DLDC-", country, xNode, SUFFIX_NONE))
                .setBus(bDc400.getId())
                .setR(0.3)
                .setX(3.0)
                .setB(0.0)
                .setG(0.0)
                .setP0(exchange)
                .setQ0(0.0)
                .setPairingKey(xNode)
                .add();
    iidm::VoltageLevel& vldc150 = s.newVoltageLevel()
                .setId(getVoltageLevelId(country, xNode, SUFFIX_150))
                .setNominalV(150.0)
                .setLowVoltageLimit(120.0)
                .setHighVoltageLimit(180.0)
                .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
                .add();

    if (mode == Mode::ONE_T2WT) {
        iidm::Bus& bDc1501 = vldc150.getBusBreakerView().newBus()
                    .setId(getBusId(country, xNode, SUFFIX_150))
                    .add();
        s.newTwoWindingsTransformer()
                    .setId(getTransformerId(country, xNode, SUFFIX_NONE))
                    .setVoltageLevel1(vldc400.getId())
                    .setBus1(bDc400.getId())
                    .setConnectableBus1(bDc400.getId())
                    .setRatedU1(400.0)
                    .setVoltageLevel2(vldc150.getId())
                    .setBus2(bDc1501.getId())
                    .setConnectableBus2(bDc1501.getId())
                    .setRatedU2(150)
                    .setR(0.1)
                    .setX(5.0)
                    .setG(0.0)
                    .setB(0.0)
                    .add();
    } else if (mode == Mode::TWO_T2WT) {
        iidm::Bus& bDc1501 = vldc150.getBusBreakerView().newBus()
                    .setId(getBusId(country, xNode, SUFFIX_150_1))
                    .add();
        iidm::Bus& bDc1502 = vldc150.getBusBreakerView().newBus()
                    .setId(getBusId(country, xNode, SUFFIX_150_2))
                    .add();
        iidm::Bus& bDc400i = vldc400.getBusBreakerView().newBus()
                    .setId(getBusId(country, xNode, SUFFIX_400_I))
                    .add();
        network.newLine()
                    .setId(getLineId(country, xNode, SUFFIX_400_I))
                    .setVoltageLevel1(vldc400.getId())
                    .setBus1(bDc400.getId())
                    .setConnectableBus1(bDc400.getId())
                    .setVoltageLevel2(vldc400.getId())
                    .setBus2(bDc400i.getId())
                    .setConnectableBus2(bDc400i.getId())
                    .setR(0.3)
                    .setX(3.0)
                    .setG1(0.0)
                    .setB1(0.0)
                    .setG2(0.0)
                    .setB2(0.0)
                    .add();
        s.newTwoWindingsTransformer()
                    .setId(getTransformerId(country, xNode, SUFFIX_1))
                    .setVoltageLevel1(vldc400.getId())
                    .setBus1(bDc400i.getId())
                    .setConnectableBus1(bDc400i.getId())
                    .setRatedU1(400.0)
                    .setVoltageLevel2(vldc150.getId())
                    .setBus2(bDc1501.getId())
                    .setConnectableBus2(bDc1501.getId())
                    .setRatedU2(150)
                    .setR(0.1)
                    .setX(5.0)
                    .setG(0.0)
                    .setB(0.0)
                    .add();
        s.newTwoWindingsTransformer()
                    .setId(getTransformerId(country, xNode, SUFFIX_2))
                    .setVoltageLevel1(vldc400.getId())
                    .setBus1(bDc400i.getId())
                    .setConnectableBus1(bDc400i.getId())
                    .setRatedU1(400.0)
                    .setVoltageLevel2(vldc150.getId())
                    .setBus2(bDc1502.getId())
                    .setConnectableBus2(bDc1502.getId())
                    .setRatedU2(150)
                    .setR(0.1)
                    .setX(5.0)
                    .setG(0.0)
                    .setB(0.0)
                    .add();
    } else if (mode == Mode::T3WT) {
        iidm::Bus& bDc1501 = vldc150.getBusBreakerView().newBus()
                    .setId(getBusId(country, xNode, SUFFIX_150_1))
                    .add();
        iidm::Bus& bDc1502 = vldc150.getBusBreakerView().newBus()
                    .setId(getBusId(country, xNode, SUFFIX_150_2))
                    .add();
        s.newThreeWindingsTransformer()
                    .setId(getTransformerId(country, xNode, SUFFIX_NONE))
                    .setRatedU0(400.0)
                    .newLeg1()
                    .setR(0.1)
                    .setX(0.0)
                    .setG(0.0)
                    .setB(0.0)
                    .setRatedU(400.0)
                    .setVoltageLevel(vldc400.getId())
                    .setBus(bDc400.getId())
                    .add()
                    .newLeg2()
                    .setR(0.1)
                    .setX(5.0)
                    .setG(0.0)
                    .setB(0.0)
                    .setRatedU(150.0)
                    .setVoltageLevel(vldc150.getId())
                    .setBus(bDc1501.getId())
                    .add()
                    .newLeg3()
                    .setR(0.1)
                    .setX(5.0)
                    .setG(0.0)
                    .setB(0.0)
                    .setRatedU(150.0)
                    .setVoltageLevel(vldc150.getId())
                    .setBus(bDc1502.getId())
                    .add()
                    .add();
    }

}

iidm::Network DcDetailedNetworkFactory::createLccMonopoleBase(const std::string& dcNetworkId) {
    std::string rootNetworkId = dcNetworkId + iidm::getCountryName(iidm::Country::FR) + iidm::getCountryName(iidm::Country::GB);
    iidm::Network rootNetwork(rootNetworkId, "test");

    iidm::Network& dcNetwork = rootNetwork.newSubnetwork(dcNetworkId, "test");
    createSimpleAcNetworkWithDanglingLines(rootNetwork, iidm::Country::FR, {{X_NODE_DC_1_FR, 200.0}});
    createSimpleAcNetworkWithDanglingLines(rootNetwork, iidm::Country::GB, {{X_NODE_DC_1_GB, -200.0}});
    addDcAcElements(dcNetwork, iidm::Country::FR, X_NODE_DC_1_FR, -200., Mode::TWO_T2WT);
    addDcAcElements(dcNetwork, iidm::Country::GB, X_NODE_DC_1_GB, 200., Mode::T3WT);

    rootNetwork.newTieLine()
                .setId("tlFr")
                .setEnsureIdUnicity(true)
                .setName("tlFr")
                .setDanglingLine1(getId("DLDC-", iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_NONE))
                .setDanglingLine2(getId("DLAC-", iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_NONE))
                .add();
    rootNetwork.newTieLine()
                .setId("tlGb")
                .setEnsureIdUnicity(true)
                .setName("tlGb")
                .setDanglingLine1(getId("DLDC-", iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_NONE))
                .setDanglingLine2(getId("DLAC-", iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_NONE))
                .add();

    iidm::DcNode& dcNodeFrPos = dcNetwork.newDcNode()
                .setId(DC_NODE_FR_POS)
                .setNominalV(500.)
                .add();
    iidm::DcNode& dcNodeFrNeg = dcNetwork.newDcNode()
                .setId(DC_NODE_FR_NEG)
                .setNominalV(1.)
                .add();
    iidm::DcNode& dcNodeGbPos = dcNetwork.newDcNode()
                .setId(DC_NODE_GB_POS)
                .setNominalV(500.)
                .add();
    iidm::DcNode& dcNodeGbNeg = dcNetwork.newDcNode()
                .setId(DC_NODE_GB_NEG)
                .setNominalV(1.)
                .add();
    dcNetwork.newDcGround()
                .setId(DC_GROUND_FR)
                .setDcNode(dcNodeFrNeg.getId())
                .setConnected(true)
                .setR(0.0)
                .add();
    dcNetwork.newDcGround()
                .setId(DC_GROUND_GB)
                .setDcNode(dcNodeGbNeg.getId())
                .setConnected(true)
                .setR(0.0)
                .add();
    dcNetwork.newDcLine()
                .setId("dcLine1")
                .setDcNode1(dcNodeFrPos.getId())
                .setConnected1(true)
                .setDcNode2(dcNodeGbPos.getId())
                .setConnected2(true)
                .setR(5.0)
                .add();
    dcNetwork.getVoltageLevel(getVoltageLevelId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_150)).newLineCommutatedConverter()
                .setId("LccFr")
                .setBus1(getBusId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_150_1))
                .setBus2(getBusId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_150_2))
                .setDcNode1(dcNodeFrNeg.getId())
                .setDcNode2(dcNodeFrPos.getId())
                .setControlMode(iidm::AcDcConverter::ControlMode::V_DC)
                .setPccTerminal(stdcxx::ref(dcNetwork.getLine(getLineId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_400_I)).getTerminal1()))
                .setTargetVdc(500.)
                .setTargetP(200.)
                .add();
    dcNetwork.getVoltageLevel(getVoltageLevelId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_150)).newLineCommutatedConverter()
                .setId("LccGb")
                .setBus1(getBusId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_150_1))
                .setBus2(getBusId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_150_2))
                .setDcNode1(dcNodeGbNeg.getId())
                .setDcNode2(dcNodeGbPos.getId())
                .setControlMode(iidm::AcDcConverter::ControlMode::P_PCC)
                .setPccTerminal(stdcxx::ref(dcNetwork.getThreeWindingsTransformer(getTransformerId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_NONE)).getLeg1().getTerminal()))
                .setTargetVdc(500.)
                .setTargetP(-200.)
                .add();

    return rootNetwork;
}

iidm::Network DcDetailedNetworkFactory::createVscMonopoleBase(const std::string& dcNetworkId) {
    std::string rootNetworkId = dcNetworkId + iidm::getCountryName(iidm::Country::FR) + iidm::getCountryName(iidm::Country::GB);
    iidm::Network rootNetwork(rootNetworkId, "test");

    iidm::Network& dcNetwork = rootNetwork.newSubnetwork(dcNetworkId, "test");
    createSimpleAcNetworkWithDanglingLines(rootNetwork, iidm::Country::FR, {{X_NODE_DC_1_FR, 200.0}});
    createSimpleAcNetworkWithDanglingLines(rootNetwork, iidm::Country::GB, {{X_NODE_DC_1_GB, -200.0}});

    addDcAcElements(dcNetwork, iidm::Country::FR, X_NODE_DC_1_FR, -200.0, Mode::ONE_T2WT);
    addDcAcElements(dcNetwork, iidm::Country::GB, X_NODE_DC_1_GB, 200.0, Mode::ONE_T2WT);

    rootNetwork.newTieLine()
                .setId("tlFr")
                .setEnsureIdUnicity(true)
                .setName("tlFr")
                .setDanglingLine1(getId("DLDC-", iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_NONE))
                .setDanglingLine2(getId("DLAC-", iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_NONE))
                .add();
    rootNetwork.newTieLine()
                .setId("tlGb")
                .setEnsureIdUnicity(true)
                .setName("tlGb")
                .setDanglingLine1(getId("DLDC-", iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_NONE))
                .setDanglingLine2(getId("DLAC-", iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_NONE))
                .add();

    iidm::DcNode& dcNodeFrPos = dcNetwork.newDcNode()
                .setId(DC_NODE_FR_POS)
                .setNominalV(250.)
                .add();
    iidm::DcNode& dcNodeFrNeg = dcNetwork.newDcNode()
                .setId(DC_NODE_FR_NEG)
                .setNominalV(250.)
                .add();
    iidm::DcNode& dcNodeGbPos = dcNetwork.newDcNode()
                .setId(DC_NODE_GB_POS)
                .setNominalV(250.)
                .add();
    iidm::DcNode& dcNodeGbNeg = dcNetwork.newDcNode()
                .setId(DC_NODE_GB_NEG)
                .setNominalV(250.)
                .add();
    dcNetwork.newDcLine()
                .setId("dcLinePos")
                .setDcNode1(dcNodeFrPos.getId())
                .setDcNode2(dcNodeGbPos.getId())
                .setR(5.0)
                .add();
    dcNetwork.newDcLine()
                .setId("dcLineNeg")
                .setDcNode1(dcNodeFrNeg.getId())
                .setDcNode2(dcNodeGbNeg.getId())
                .setR(5.0)
                .add();
    iidm::Terminal& frPccTerminal = dcNetwork.getTwoWindingsTransformer(getTransformerId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_NONE)).getTerminal1();
    dcNetwork.getVoltageLevel(getVoltageLevelId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_150)).newVoltageSourceConverter()
                .setId("VscFr")
                .setBus1(getBusId(iidm::Country::FR, X_NODE_DC_1_FR, SUFFIX_150))
                .setDcNode1(dcNodeFrNeg.getId())
                .setDcNode2(dcNodeFrPos.getId())
                .setControlMode(iidm::AcDcConverter::ControlMode::V_DC)
                .setPccTerminal(stdcxx::ref(frPccTerminal))
                .setTargetVdc(500.)
                .setTargetP(200.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .setVoltageSetpoint(400.)
                .add();
    iidm::Terminal& gbPccTerminal = dcNetwork.getTwoWindingsTransformer(getTransformerId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_NONE)).getTerminal1();
    dcNetwork.getVoltageLevel(getVoltageLevelId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_150)).newVoltageSourceConverter()
                .setId("VscGb")
                .setBus1(getBusId(iidm::Country::GB, X_NODE_DC_1_GB, SUFFIX_150))
                .setDcNode1(dcNodeGbNeg.getId())
                .setDcNode2(dcNodeGbPos.getId())
                .setControlMode(iidm::AcDcConverter::ControlMode::P_PCC)
                .setPccTerminal(stdcxx::ref(gbPccTerminal))
                .setTargetVdc(500.)
                .setTargetP(-200.)
                .setVoltageRegulatorOn(false)
                .setReactivePowerSetpoint(0.0)
                .setVoltageSetpoint(400.)
                .add();

    return rootNetwork;
}

}  // namespace network

}  // namespace powsybl
