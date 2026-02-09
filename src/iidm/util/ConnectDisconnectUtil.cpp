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
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace ConnectDisconnectUtil {

bool connectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) {
    bool isAlreadyConnected = true;
    bool isNowConnected = true;

    //Check connected state of terminals
    for (auto& terminal : terminals) {
        if (!terminal.get().isConnected()) {
            isAlreadyConnected = false;
        }
    }
    // Exit if the connectable is already fully connected
    if(isAlreadyConnected) {
        return false;
    }

    //Try connecting all disconnected terminals
    for (auto& terminal : terminals) {
        if (terminal.get().isConnected()) {
            continue;
        }
        isNowConnected = isNowConnected && terminal.get().connect(isTypeSwitchToOperate);
        // Exit if the terminal cannot be connected
        if (!isNowConnected) {
            return false;
        }
    }
    return isNowConnected;
}

bool disconnectAllTerminals(std::vector<std::reference_wrapper<Terminal>> terminals, const stdcxx::Predicate<Switch>& isTypeSwitchOpenable) {
    bool isAlreadyDisconnected = true;
    bool isNowDisconnected = true;

    //Check connected state of terminals
    for (auto& terminal : terminals) {
        if (terminal.get().isConnected()) {
            isAlreadyDisconnected = false;
        }
    }
    // Exit if the connectable is already fully disconnected
    if(isAlreadyDisconnected) {
        return false;
    }

    //We try to disconnect each connected terminal
    for (auto& terminal : terminals) {
        if (!terminal.get().isConnected()) {
            continue;
        }
        isNowDisconnected = isNowDisconnected && terminal.get().disconnect(isTypeSwitchOpenable);
        // Exit if the terminal cannot be disconnected
        if (!isNowDisconnected) {
            return false;
        }
    }
    return isNowDisconnected;
}


}  // namespace ConnectDisconnectUtil

}  // namespace iidm

}  // namespace powsybl
