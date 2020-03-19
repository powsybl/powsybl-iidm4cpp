/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINALBUILDER_HPP
#define POWSYBL_IIDM_TERMINALBUILDER_HPP

#include <memory>
#include <string>

#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class Validable;
class VoltageLevel;

class IIDM_DECLSPEC TerminalBuilder {
public:
    TerminalBuilder(VoltageLevel& voltageLevel, Validable& validable);

    ~TerminalBuilder() noexcept = default;

    std::unique_ptr<Terminal> build();

    TerminalBuilder& setBus(const std::string& bus);

    TerminalBuilder& setConnectableBus(const std::string& connectableBus);

    TerminalBuilder& setNode(const stdcxx::optional<unsigned long>& node);

private:
    const std::string& getConnectionBus() const;

private:
    VoltageLevel& m_voltageLevel;

    Validable& m_validable;

    stdcxx::optional<unsigned long> m_node;

    std::string m_bus;

    std::string m_connectableBus;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TERMINALBUILDER_HPP
