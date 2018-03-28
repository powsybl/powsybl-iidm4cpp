/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SWITCHADDER_HPP
#define POWSYBL_IIDM_SWITCHADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/iidm/SwitchKind.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Switch;

class VoltageLevel;

template <typename SwitchAdder>
class AbstractSwitchAdder : public IdentifiableAdder<SwitchAdder> {
public:
    virtual ~AbstractSwitchAdder() = default;

    virtual Switch& add() = 0;

    SwitchAdder& setFictitious(bool fictitious);

    SwitchAdder& setOpen(bool open);

protected: // IdentifiableAdder
    virtual Network& getNetwork() override;

protected:
    AbstractSwitchAdder(VoltageLevel& voltageLevel);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

protected:
    VoltageLevel& m_voltageLevel;

    bool m_open;

    bool m_fictitious;
};

namespace node_breaker_view {

class SwitchAdder : public AbstractSwitchAdder<SwitchAdder> {
public: // AbstractSwitchAdder
    Switch& add() override;

public:
    SwitchAdder(VoltageLevel& voltageLevel);

    virtual ~SwitchAdder() = default;

    SwitchAdder& setKind(const SwitchKind& kind);

    SwitchAdder& setNode1(unsigned long node1);

    SwitchAdder& setNode2(unsigned long node2);

    SwitchAdder& setRetained(bool retained);

private:
    stdcxx::optional<unsigned long> m_node1;

    stdcxx::optional<unsigned long> m_node2;

    stdcxx::optional<SwitchKind> m_kind;

    bool m_retained;
};

}

namespace bus_breaker_view {

class SwitchAdder : public AbstractSwitchAdder<SwitchAdder> {
public: // AbstractSwitchAdder
    Switch& add() override;

public:
    SwitchAdder(VoltageLevel& voltageLevel);

    virtual ~SwitchAdder() = default;

    SwitchAdder& setBus1(const std::string& bus1);

    SwitchAdder& setBus2(const std::string& bus2);

private:
    stdcxx::optional<std::string> m_bus1;

    stdcxx::optional<std::string> m_bus2;
};

}

}

}

#include <powsybl/iidm/SwitchAdder.hxx>

#endif  // POWSYBL_IIDM_SWITCHADDER_HPP
