/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODETERMINALVIEWS_HPP
#define POWSYBL_IIDM_NODETERMINALVIEWS_HPP

#include <powsybl/iidm/TerminalViews.hpp>

namespace powsybl {

namespace iidm {

class NodeTerminal;

namespace node_terminal {

class BusBreakerViewImpl : public terminal::BusBreakerView {
public: // BusBreakerView
    stdcxx::Reference<Bus> getBus() const override;

    stdcxx::Reference<Bus> getConnectableBus() const override;

    void setConnectableBus(const std::string& busId) override;

public:
    explicit BusBreakerViewImpl(NodeTerminal& terminal);

    ~BusBreakerViewImpl() noexcept override = default;

private:
    NodeTerminal& m_terminal;
};

class BusViewImpl : public terminal::BusView {
public: // BusView
    stdcxx::Reference<Bus> getBus() const override;

    stdcxx::Reference<Bus> getConnectableBus() const override;

public:
    explicit BusViewImpl(NodeTerminal& terminal);

    ~BusViewImpl() noexcept override = default;

private:
    NodeTerminal& m_terminal;
};

class NodeBreakerViewImpl : public terminal::NodeBreakerView {
public: // NodeBreakerView
    unsigned long getNode() const override;

public:
    explicit NodeBreakerViewImpl(NodeTerminal& terminal);

    ~NodeBreakerViewImpl() noexcept override = default;

private:
    NodeTerminal& m_terminal;
};

}  // namespace node_terminal

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODETERMINALVIEWS_HPP
