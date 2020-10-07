/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_NETWORK_FICTITIOUSSWITCHFACTORY_HPP
#define POWSYBL_NETWORK_FICTITIOUSSWITCHFACTORY_HPP

#include <string>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/SwitchKind.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;

}  // namespace iidm

namespace network {

class FictitiousSwitchFactory {
public:
    static iidm::Network create();

public:
    FictitiousSwitchFactory() = delete;

private:
    static void createSwitch(iidm::VoltageLevel& vl, const std::string& id, const std::string& name, const iidm::SwitchKind& kind, bool retained, bool open, bool fictitious, unsigned long node1, unsigned long node2);
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_NETWORK_FICTITIOUSSWITCHFACTORY_HPP
