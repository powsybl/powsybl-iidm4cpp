/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>


#include <powsybl/iidm/AbstractTerminalTopologyVisitor.hpp>
#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/network/DcDetailedNetworkFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace network {

BOOST_AUTO_TEST_SUITE(DcDetailedNetworkFactoryTestSuite)

BOOST_AUTO_TEST_CASE(testLccMonopoleGroundReturn) {
    iidm::Network network = DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    iidm::Network& dcNetwork = network.getSubNetwork("LccMonopoleGroundReturn").get();

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getDcComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(dcNetwork.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(dcNetwork.getDcComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(dcNetwork.getBusView().getConnectedComponents()));

    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(4, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(1, network.getDcLineCount());
    BOOST_CHECK_EQUAL(2, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    BOOST_CHECK(network.getDcGround("dcGroundFr").getDcTerminal().isConnected());
    BOOST_CHECK(network.getDcGround("dcGroundGb").getDcTerminal().isConnected());

}

BOOST_AUTO_TEST_CASE(testLccMonopoleMetallicReturn) {
    iidm::Network  network = DcDetailedNetworkFactory::createLccMonopoleMetallicReturn();
    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getDcComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(4, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(2, network.getDcLineCount());
    BOOST_CHECK_EQUAL(2, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    BOOST_CHECK(network.getDcGround("dcGroundFr").getDcTerminal().isConnected());
    BOOST_CHECK(!network.getDcGround("dcGroundGb").getDcTerminal().isConnected());
}

BOOST_AUTO_TEST_CASE(testVscSymmetricalMonopole) {
    iidm::Network  network = DcDetailedNetworkFactory::createVscSymmetricalMonopole();
    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getDcComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(4, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(2, network.getDcLineCount());
    BOOST_CHECK_EQUAL(2, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(0, network.getDcGroundCount());
}

BOOST_AUTO_TEST_CASE(testVscAsymmetricalMonopole) {
    iidm::Network  network = DcDetailedNetworkFactory::createVscAsymmetricalMonopole();
    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getDcComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(4, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(1, network.getDcLineCount());
    BOOST_CHECK_EQUAL(2, network.getVoltageSourceConverterCount());
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
}

BOOST_AUTO_TEST_CASE(testLccBipoleGroundReturn) {
    iidm::Network network = DcDetailedNetworkFactory::createLccBipoleGroundReturn();

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getDcComponents()));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(6, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(4, network.getDcSwitchCount());
    BOOST_CHECK_EQUAL(2, network.getDcLineCount());
    BOOST_CHECK_EQUAL(4, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    BOOST_CHECK_EQUAL(6, network.getDcBusCount());
}

BOOST_AUTO_TEST_CASE(testLccBipoleGroundReturnNegativePoleOutage) {
    iidm::Network network = DcDetailedNetworkFactory::createLccBipoleGroundReturnNegativePoleOutage();

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    auto dcComponents = network.getDcComponents();
    BOOST_CHECK_EQUAL(1, boost::size(dcComponents));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(6, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(4, network.getDcSwitchCount());
    BOOST_CHECK_EQUAL(2, network.getDcLineCount());
    BOOST_CHECK_EQUAL(4, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    BOOST_CHECK_EQUAL(4, network.getDcBusCount());

    auto& dcComponent0 = dcComponents.front();
    BOOST_CHECK_EQUAL(4, dcComponent0.getSize());
}

BOOST_AUTO_TEST_CASE(testLccBipoleGroundReturnWithDcLineSegments) {
    iidm::Network network = DcDetailedNetworkFactory::createLccBipoleGroundReturnWithDcLineSegments();

    BOOST_CHECK_EQUAL(2, boost::size(network.getBusView().getSynchronousComponents()));
    auto dcComponents = network.getDcComponents();
    BOOST_CHECK_EQUAL(1, boost::size(dcComponents));
    BOOST_CHECK_EQUAL(1, boost::size(network.getBusView().getConnectedComponents()));
    BOOST_CHECK_EQUAL(3, boost::size(network.getSubNetworks()));
    BOOST_CHECK_EQUAL(22, network.getDcNodeCount());
    BOOST_CHECK_EQUAL(12, network.getDcSwitchCount());
    BOOST_CHECK_EQUAL(12, network.getDcLineCount());
    BOOST_CHECK_EQUAL(4, network.getLineCommutatedConverterCount());
    BOOST_CHECK_EQUAL(2, network.getDcGroundCount());
    BOOST_CHECK_EQUAL(14, network.getDcBusCount());

    auto& dcComponent0 = dcComponents.front();
    BOOST_CHECK_EQUAL(14, dcComponent0.getSize());
}


class TerminalTopologyVisitor : public iidm::AbstractTerminalTopologyVisitor {
public:
    TerminalTopologyVisitor() = default;

    const std::map<iidm::IdentifiableType, std::vector<std::string>>& getConnectables() const {
        return m_equipments;
    }

    const std::map<iidm::TerminalNumber, std::vector<stdcxx::Reference<iidm::Terminal>>>& getTerminals() const {
        return m_terminals;
    }

    void visitTerminal(const iidm::Terminal& terminal) override {
        if(stdcxx::isInstanceOf<iidm::AcDcConverter>(terminal.getConnectable())) {
            m_equipments[terminal.getConnectable().get().getType()].push_back(terminal.getConnectable().get().getId());
            if(terminal.getTerminalNumber() != iidm::TerminalNumber::UNDEFINED) {
                m_terminals[terminal.getTerminalNumber()].push_back(stdcxx::ref(terminal));
            }
            
        }
    }

private:
    std::map<iidm::IdentifiableType, std::vector<std::string>> m_equipments;
    std::map<iidm::TerminalNumber, std::vector<stdcxx::Reference<iidm::Terminal>>> m_terminals;
};

BOOST_AUTO_TEST_CASE(topologyVisitorTest) {
    iidm::Network network = DcDetailedNetworkFactory::createLccMonopoleGroundReturn();

    TerminalTopologyVisitor visitor;

    network.getVoltageLevel(DcDetailedNetworkFactory::getVoltageLevelId(iidm::Country::FR, DcDetailedNetworkFactory::X_NODE_DC_1_FR, DcDetailedNetworkFactory::SUFFIX_150))
        .visitEquipments(visitor);

    auto visited = visitor.getConnectables();

    BOOST_CHECK(visited.count(iidm::IdentifiableType::LINE_COMMUTATED_CONVERTER)>0);
    BOOST_CHECK_EQUAL(2, visited.at(iidm::IdentifiableType::LINE_COMMUTATED_CONVERTER).size());
    for (auto& lccId : visited.at(iidm::IdentifiableType::LINE_COMMUTATED_CONVERTER)) {
        BOOST_CHECK_EQUAL("LccFr", lccId);
    }
    auto& lcc = network.getLineCommutatedConverter("LccFr");
    for (auto& terminal : visitor.getTerminals()) {
        BOOST_CHECK_EQUAL(1, terminal.second.size());
        BOOST_CHECK(stdcxx::areSame(terminal.second[0].get().getConnectable().get(), lcc));
        BOOST_CHECK(stdcxx::areSame(lcc.getTerminal(static_cast<iidm::TerminalNumber>(terminal.first)), terminal.second[0].get()));
    }
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace network

}  // namespace powsybl
