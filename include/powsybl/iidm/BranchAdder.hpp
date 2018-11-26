/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BRANCHADDER_HPP
#define POWSYBL_IIDM_BRANCHADDER_HPP

#include <memory>

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class VoltageLevel;

template<typename Adder>
class BranchAdder : public IdentifiableAdder<Adder> {
public:
    BranchAdder() = default;

    ~BranchAdder() noexcept override = default;

    Adder& setBus1(const std::string& bus1);

    Adder& setBus2(const std::string& bus2);

    Adder& setConnectableBus1(const std::string& connectableBus1);

    Adder& setConnectableBus2(const std::string& connectableBus2);

    Adder& setNode1(unsigned long node1);

    Adder& setNode2(unsigned long node2);

    Adder& setVoltageLevel1(const std::string& voltageLevelId1);

    Adder& setVoltageLevel2(const std::string& voltageLevelId2);

protected:
    VoltageLevel& checkAndGetVoltageLevel1();

    VoltageLevel& checkAndGetVoltageLevel2();

    std::unique_ptr<Terminal> getTerminal1();

    std::unique_ptr<Terminal> getTerminal2();

private:
    std::string m_bus1;

    std::string m_bus2;

    std::string m_connectableBus1;

    std::string m_connectableBus2;

    stdcxx::optional<unsigned long> m_node1;

    stdcxx::optional<unsigned long> m_node2;

    std::string m_voltageLevelId1;

    std::string m_voltageLevelId2;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/BranchAdder.hxx>

#endif  // POWSYBL_IIDM_BRANCHADDER_HPP
