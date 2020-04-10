/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INJECTIONADDER_HPP
#define POWSYBL_IIDM_INJECTIONADDER_HPP

#include <memory>

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class VoltageLevel;

template <typename Adder>
class InjectionAdder : public IdentifiableAdder<Adder> {
public:
    ~InjectionAdder() noexcept override = default;

    Adder& setBus(const std::string& bus);

    Adder& setConnectableBus(const std::string& connectableBus);

    Adder& setNode(unsigned long node);

protected:  // IdentifiableAdder
    Network& getNetwork() override;

protected:
    InjectionAdder(VoltageLevel& voltageLevel);

    std::unique_ptr<Terminal> checkAndGetTerminal();

    VoltageLevel& getVoltageLevel();

private:
    VoltageLevel& m_voltageLevel;

    stdcxx::optional<unsigned long> m_node;

    std::string m_bus;

    std::string m_connectableBus;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/InjectionAdder.hxx>

#endif  // POWSYBL_IIDM_INJECTIONADDER_HPP
