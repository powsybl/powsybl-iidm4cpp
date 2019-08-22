/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSTERMINALVIEWS_HPP
#define POWSYBL_IIDM_BUSTERMINALVIEWS_HPP

#include <powsybl/iidm/TerminalViews.hpp>

namespace powsybl {

namespace iidm {

class BusTerminal;

namespace bus_terminal {

class BusBreakerViewImpl : public terminal::BusBreakerView {
public: // BusBreakerView
    stdcxx::Reference<Bus> getBus() const override;

    stdcxx::Reference<Bus> getConnectableBus() const override;

    void setConnectableBus(const std::string& busId) override;

public:
    explicit BusBreakerViewImpl(BusTerminal& terminal);

    ~BusBreakerViewImpl() noexcept override = default;

private:
    BusTerminal& m_terminal;
};

class BusViewImpl : public terminal::BusView {
public: // BusView
    stdcxx::Reference<Bus> getBus() const override;

    stdcxx::Reference<Bus> getConnectableBus() const override;

public:
    explicit BusViewImpl(BusTerminal& terminal);

    ~BusViewImpl() noexcept override = default;

private:
    BusTerminal& m_terminal;
};

}  // namespace bus_terminal

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSTERMINALVIEWS_HPP
