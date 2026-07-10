/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/DcConnectable.hpp>
// #include <powsybl/iidm/DcGroundAdder.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcSwitch.hpp>
// #include <powsybl/iidm/DcTopologyVisitor.hpp>
#include <powsybl/iidm/DcTerminalTopologyTraverser.hpp>
// #include <powsybl/iidm/Enum.hpp>
// #include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/Network.hpp>
// #include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>

#include <powsybl/network/DcDetailedNetworkFactory.hpp>

// #include <powsybl/stdcxx/reference.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DcTopologyTraverserTestSuite)

class CustomConverterDcTopologyTraverser : public DcTerminal::DcTopologyTraverser {
public:
    CustomConverterDcTopologyTraverser(bool onlyConnectedDcNode) : 
        DcTerminal::DcTopologyTraverser(),
        m_onlyConnectedDcNode(onlyConnectedDcNode) {
    }

    // DcTopologyTraverser
    math::TraverseResult traverse(DcTerminal& dcTerminal, bool /*connected*/) override {
        if(DcTerminal::isInstanceOf<AcDcConverter>(dcTerminal)) {
            m_firstTraversedConverterId = dcTerminal.getDcConnectable().get().getId();
            return math::TraverseResult::TERMINATE_TRAVERSER;
        }
        return math::TraverseResult::CONTINUE;
    }
    math::TraverseResult traverse(DcSwitch& dcSwitch) override {
        if(m_onlyConnectedDcNode && dcSwitch.isOpen()) {
            return math::TraverseResult::TERMINATE_PATH;
        }
        return math::TraverseResult::CONTINUE;
    }

    const std::string& getFirstTraversedConverterId() {
        return m_firstTraversedConverterId;
    }

private:
    bool m_onlyConnectedDcNode;
    std::string m_firstTraversedConverterId;
};

std::string getConverterSectionId(DcTerminal& terminal) {
    CustomConverterDcTopologyTraverser connectedConverter(terminal.isConnected());
    terminal.traverse(connectedConverter);
    return connectedConverter.getFirstTraversedConverterId();
}

class CustomLineDcTopologyTraverser : public DcTerminal::DcTopologyTraverser {
public:
    CustomLineDcTopologyTraverser(bool onlyConnectedDcNode) : 
        DcTerminal::DcTopologyTraverser(),
        m_onlyConnectedDcNode(onlyConnectedDcNode) {
    }

    // DcTopologyTraverser
    math::TraverseResult traverse(DcTerminal& dcTerminal, bool /*connected*/) override {
        if(DcTerminal::isInstanceOf<DcLine>(dcTerminal)) {
            m_firstTraversedDcLineId = dcTerminal.getDcConnectable().get().getId();
            return math::TraverseResult::TERMINATE_TRAVERSER;
        }
        return math::TraverseResult::CONTINUE;
    }
    math::TraverseResult traverse(DcSwitch& dcSwitch) override {
        if(m_onlyConnectedDcNode && dcSwitch.isOpen()) {
            return math::TraverseResult::TERMINATE_PATH;
        }
        return math::TraverseResult::CONTINUE;
    }

    const std::string& getFirstTraversedDcLineId() {
        return m_firstTraversedDcLineId;
    }

private:
    bool m_onlyConnectedDcNode;
    std::string m_firstTraversedDcLineId;
};

std::string getDcLineSectionId(DcTerminal& terminal) {
    CustomLineDcTopologyTraverser connectedDcLine(terminal.isConnected());
    terminal.traverse(connectedDcLine);
    return connectedDcLine.getFirstTraversedDcLineId();
}

BOOST_AUTO_TEST_CASE(testVscSymmetricalMonopole) {
    Network network = powsybl::network::DcDetailedNetworkFactory::createVscSymmetricalMonopole();

    BOOST_CHECK_EQUAL("VscGb", getConverterSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_NEG).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("VscGb", getConverterSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_POS).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("VscFr", getConverterSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_NEG).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("VscFr", getConverterSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("VscFr", getConverterSectionId(network.getDcLine("dcLineNeg").getDcTerminal1()));
    BOOST_CHECK_EQUAL("VscGb", getConverterSectionId(network.getDcLine("dcLineNeg").getDcTerminal2()));
    BOOST_CHECK_EQUAL("VscFr", getConverterSectionId(network.getDcLine("dcLinePos").getDcTerminal1()));
    BOOST_CHECK_EQUAL("VscGb", getConverterSectionId(network.getDcLine("dcLinePos").getDcTerminal2()));
    BOOST_CHECK_EQUAL("VscFr", getConverterSectionId(network.getVoltageSourceConverter("VscFr").getDcTerminal1()));
    BOOST_CHECK_EQUAL("VscGb", getConverterSectionId(network.getVoltageSourceConverter("VscGb").getDcTerminal1()));

    BOOST_CHECK_EQUAL("dcLineNeg", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_NEG).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLinePos", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_POS).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLineNeg", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_NEG).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLinePos", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLineNeg", getDcLineSectionId(network.getDcLine("dcLineNeg").getDcTerminal1()));
    BOOST_CHECK_EQUAL("dcLineNeg", getDcLineSectionId(network.getDcLine("dcLineNeg").getDcTerminal2()));
    BOOST_CHECK_EQUAL("dcLinePos", getDcLineSectionId(network.getDcLine("dcLinePos").getDcTerminal1()));
    BOOST_CHECK_EQUAL("dcLinePos", getDcLineSectionId(network.getDcLine("dcLinePos").getDcTerminal2()));
    BOOST_CHECK_EQUAL("dcLineNeg", getDcLineSectionId(network.getVoltageSourceConverter("VscFr").getDcTerminal1()));
    BOOST_CHECK_EQUAL("dcLineNeg", getDcLineSectionId(network.getVoltageSourceConverter("VscGb").getDcTerminal1()));

}

BOOST_AUTO_TEST_CASE(testLccBipoleGroundReturn) {
    Network network = powsybl::network::DcDetailedNetworkFactory::createLccBipoleGroundReturn();

    network.getDcSwitch("dcSwitchGbPosBypass").setOpen(false);
    network.getDcSwitch("dcSwitchFrPosBypass").setOpen(false);

    BOOST_CHECK_EQUAL("dcLine2", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_NEG).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLine1", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_POS).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLine2", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_NEG).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLine1", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_POS).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLine1", getDcLineSectionId(network.getDcLine("dcLine1").getDcTerminal1()));
    BOOST_CHECK_EQUAL("dcLine1", getDcLineSectionId(network.getDcLine("dcLine1").getDcTerminal2()));
    BOOST_CHECK_EQUAL("dcLine2", getDcLineSectionId(network.getDcLine("dcLine2").getDcTerminal1()));
    BOOST_CHECK_EQUAL("dcLine2", getDcLineSectionId(network.getDcLine("dcLine2").getDcTerminal2()));
    BOOST_CHECK_EQUAL("dcLine1", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_GB_MID).getDcTerminals().front()));
    BOOST_CHECK_EQUAL("dcLine1", getDcLineSectionId(network.getDcNode(powsybl::network::DcDetailedNetworkFactory::DC_NODE_FR_MID).getDcTerminals().front()));
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
