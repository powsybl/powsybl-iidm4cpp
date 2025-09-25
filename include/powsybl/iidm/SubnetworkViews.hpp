/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SUBNETWORKVIEWS_HPP
#define POWSYBL_IIDM_SUBNETWORKVIEWS_HPP

#include <string>

#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/NetworkViews.hpp>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class BusBreakerVoltageLevel;
class Network;
class NodeBreakerVoltageLevel;
class Switch;

namespace subnetwork {

class BusBreakerView : public network::BusBreakerView {
public:
    explicit BusBreakerView(Network& network);

    ~BusBreakerView() noexcept = default;

    stdcxx::CReference<Bus> getBus(const std::string& id) const override;

    stdcxx::Reference<Bus> getBus(const std::string& id) override;
};

class BusView : public network::BusView {
public:
    explicit BusView(Network& network);

    ~BusView() noexcept = default;

    stdcxx::CReference<Bus> getBus(const std::string& id) const override;

    stdcxx::Reference<Bus> getBus(const std::string& id) override;

    stdcxx::const_range<Component> getConnectedComponents() const override;

    stdcxx::range<Component> getConnectedComponents() override;

    stdcxx::const_range<Component> getSynchronousComponents() const override;

    stdcxx::range<Component> getSynchronousComponents() override;
};

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SUBNETWORKVIEWS_HPP
