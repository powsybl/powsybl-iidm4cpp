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

template <typename Adder>
class InjectionAdder : public IdentifiableAdder<Adder> {
public:
    virtual ~InjectionAdder() = default;

    Adder& setBus(const std::string& bus);

    Adder& setConnectableBus(const std::string& connectableBus);

    Adder& setNode(int node);

protected:
    InjectionAdder() = default;

    std::unique_ptr<Terminal> getTerminal();

private:
    const std::string& getConnectionBus() const;

private:
    stdcxx::optional<int> m_node;

    std::string m_bus;

    std::string m_connectableBus;
};

}

}

#include <powsybl/iidm/InjectionAdder.hxx>

#endif  // POWSYBL_IIDM_INJECTIONADDER_HPP
