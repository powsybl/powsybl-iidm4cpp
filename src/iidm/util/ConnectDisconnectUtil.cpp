/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/ConnectDisconnectUtil.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

#include "../NodeBreakerTopologyModel.hpp"

namespace powsybl {

namespace iidm {

namespace ConnectDisconnectUtil {

bool connectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    bool isAlreadyConnected = true;
    bool isNowConnected = true;

    //list of nodebreaker view switches to connect
    std::vector<stdcxx::Reference<Switch>> switchesToConnect;

    //Check connected state of terminals
    for (auto& terminal : terminals) {
        if(terminal.get().isConnected()) {
            continue;
        }

        //not already connected :
        isAlreadyConnected = false;
        //If it's a nodebreaker terminal, get the switches to connect :
        if(terminal.get().getVoltageLevel().getTopologyKind() == TopologyKind::NODE_BREAKER) {
            auto& topologyModel = terminal.get().getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();
            isNowConnected = topologyModel.getConnectingSwitches(terminal.get() , isTypeSwitchToOperate, switchesToConnect); 
        }
        //in busbreaker, nothing to do here

        if(!isNowConnected) { //Cannot connect this nodebreaker terminal, return immediatly
            return false;
        }
    }
    // Exit if the connectable is already fully connected
    if(isAlreadyConnected) {
        return false;
    }

    //Connect all disconnected busbreaker terminals
    for (auto& terminal : terminals) {
        if (terminal.get().isConnected()) {
            continue;
        }
        if(terminal.get().getVoltageLevel().getTopologyKind() == TopologyKind::BUS_BREAKER) {
            // at this point isNowConnected should always stay true
            isNowConnected = isNowConnected && terminal.get().connect(isTypeSwitchToOperate);
        }
        // Exit if the terminal cannot be connected
        if (!isNowConnected) {
            return false;
        }
    }
    //Connect all the switches for nodebreaker terminals
    for (const auto& sw : switchesToConnect) { // no need to reverify the predicate here, it is done while fetching them
        sw.get().setOpen(false);
    }

    return isNowConnected;
}

bool disconnectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchOpenable) {
    bool isAlreadyDisconnected = true;
    bool isNowDisconnected = true;

    //list of nodebreaker view switches to disconnect
    std::vector<stdcxx::Reference<Switch>> switchesToDisconnect;

    //Check connected state of terminals
    for (auto& terminal : terminals) {
        if (!terminal.get().isConnected()) {
            continue; //terminal already disconnected
        }

        isAlreadyDisconnected = false;
        //If it's a nodebreaker terminal, get the switches to disconnect :
        if(terminal.get().getVoltageLevel().getTopologyKind() == TopologyKind::NODE_BREAKER) {
            auto& topologyModel = terminal.get().getVoltageLevel().getTopologyModel<NodeBreakerTopologyModel>();
            isNowDisconnected = topologyModel.getDisconnectingSwitches(terminal.get() , isTypeSwitchOpenable, switchesToDisconnect); 
        }
        //in busbreaker, nothing to do here

        if(!isNowDisconnected) { //Cannot disconnect this (nodebreaker) terminal, return immediatly
            return false;
        }
    }
    // Exit if the connectable is already fully disconnected
    if(isAlreadyDisconnected) {
        return false;
    }

    //Disconnect all busbreaker terminals
    for (auto& terminal : terminals) {
        if (!terminal.get().isConnected()) {
            continue;
        }
        if(terminal.get().getVoltageLevel().getTopologyKind() == TopologyKind::BUS_BREAKER) {
            // at this point isNowDisconnected should always stay true
            isNowDisconnected = isNowDisconnected && terminal.get().disconnect(isTypeSwitchOpenable);
        }
        // Exit if the terminal cannot be disconnected
        if (!isNowDisconnected) {
            return false;
        }
    }
    //Disconnect all the identified switches for nodebreaker terminals
    for (const auto& sw : switchesToDisconnect) { // no need to reverify the predicate here, it is done while fetching them
        sw.get().setOpen(true);
    }
    return isNowDisconnected;
}


}  // namespace ConnectDisconnectUtil

}  // namespace iidm

}  // namespace powsybl
