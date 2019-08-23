/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINALVIEWS_HPP
#define POWSYBL_IIDM_TERMINALVIEWS_HPP

#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;

namespace terminal {

class BusBreakerView {
public:
    virtual ~BusBreakerView() noexcept = default;

    virtual stdcxx::CReference<Bus> getBus() const = 0;

    virtual stdcxx::Reference<Bus> getBus() = 0;

    virtual stdcxx::CReference<Bus> getConnectableBus() const = 0;

    virtual stdcxx::Reference<Bus> getConnectableBus() = 0;

    virtual void setConnectableBus(const std::string& busId) = 0;
};

class BusView {
public:
    virtual ~BusView() noexcept = default;

    virtual stdcxx::CReference<Bus> getBus() const = 0;

    virtual stdcxx::Reference<Bus> getBus() = 0;

    virtual stdcxx::CReference<Bus> getConnectableBus() const = 0;

    virtual stdcxx::Reference<Bus> getConnectableBus() = 0;
};

class NodeBreakerView {
public:
    virtual ~NodeBreakerView() noexcept = default;

    virtual unsigned long getNode() const = 0;
};

}  // namespace terminal

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TERMINALVIEWS_HPP
