/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

TieLineAdder::TieLineAdder(Network& network) :
    m_network(network) {
}

TieLine& TieLineAdder::add() {
    
    if(m_dlId1.empty() || m_dlId2.empty()) {
        throw ValidationException(*this, "undefined dangling line");
    }

    DanglingLine& dl1 = m_network.getDanglingLine(m_dlId1);
    DanglingLine& dl2 = m_network.getDanglingLine(m_dlId2);

    if(stdcxx::areSame(dl1, dl2)) {
        throw ValidationException(*this, stdcxx::format("danglingLine1 and danglingLine2 are identical (%1%)", m_dlId1));
    }

    if (static_cast<bool>(dl1.getTieLine())) {
        throw ValidationException(*this, stdcxx::format("danglingLine1 (%1%) already has a tie line", m_dlId1));
    } else if (static_cast<bool>(dl2.getTieLine())) {
        throw ValidationException(*this, stdcxx::format("danglingLine2 (%1%) already has a tie line", m_dlId2));
    }

    if (!dl1.getPairingKey().empty() && !dl2.getPairingKey().empty() && dl1.getPairingKey() != dl2.getPairingKey()) {
        throw ValidationException(*this, "pairingKey is not consistent");
    }
    
    std::unique_ptr<TieLine> ptrTieLine = std::unique_ptr<TieLine>(new TieLine(m_network, checkAndGetUniqueId(), getName(), isFictitious()));
    auto& tieLine = m_network.checkAndAdd<TieLine>(std::move(ptrTieLine));
    tieLine.attachDanglingLines(dl1, dl2);

    m_network.getConnectedComponentsManager().invalidate();
    m_network.getSynchronousComponentsManager().invalidate();

    return tieLine;
}

const Network& TieLineAdder::getNetwork() const {
    return m_network;
}

Network& TieLineAdder::getNetwork() {
    return m_network;
}

const std::string& TieLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "AC tie line";

    return s_typeDescription;
}

TieLineAdder& TieLineAdder::setDanglingLine1(const std::string& id) {
    m_dlId1 = id;
    return *this;
}

TieLineAdder& TieLineAdder::setDanglingLine2(const std::string& id) {
    m_dlId2 = id;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
