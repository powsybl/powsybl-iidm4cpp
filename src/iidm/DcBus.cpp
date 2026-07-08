/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcBus.hpp>

#include <cassert>

#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcTerminal.hpp>

#include <powsybl/stdcxx/flattened.hpp>
#include <powsybl/stdcxx/math.hpp>


namespace powsybl {

namespace iidm {


DcBus::DcBus(const std::string& id, const std::string& name, bool fictitious, DcNodeSet dcNodes) :
    Identifiable(id, name, fictitious),
    m_dcNodes(std::move(dcNodes)) {
        if(m_dcNodes.empty()) {
            throw PowsyblException(stdcxx::format("DcBus '%1%' set of DcNodes is empty", id));
        }
}

void DcBus::checkValidity() const {
    if (!m_valid) {
        throw PowsyblException("DcBus has been invalidated");
    }
}

const Network& DcBus::getNetwork() const {
    checkValidity();
    if(m_dcNodes.empty()) {
        throw PowsyblException(stdcxx::format("DcBus '%1%' set of DcNodes is empty", getId()));
    }
    return m_dcNodes.front().get().getNetwork();
}

Network& DcBus::getNetwork() {
    return const_cast<Network&>(static_cast<const DcBus*>(this)->getNetwork());
}

const Network& DcBus::getParentNetwork() const {
    checkValidity();
    if(m_dcNodes.empty()) {
        throw PowsyblException(stdcxx::format("DcBus '%1%' set of DcNodes is empty", getId()));
    }
    return m_dcNodes.front().get().getParentNetwork();
}

Network& DcBus::getParentNetwork() {
    return const_cast<Network&>(static_cast<const DcBus*>(this)->getParentNetwork());
}

const IdentifiableType& DcBus::getType() const {
    static IdentifiableType s_type = IdentifiableType::DC_BUS;
    return s_type;
}

const std::string& DcBus::getTypeDescription() const {
    static std::string s_typeDescription = "DC Bus";
    return s_typeDescription;
}


double DcBus::getV() {
    checkValidity();
    for (const auto& n : m_dcNodes) {
        if (!std::isnan(n.get().getV())) {
            return n.get().getV();
        }
    }
    return stdcxx::nan();
}
DcBus& DcBus::setV(double v) {
    checkValidity();
    for (auto& n : m_dcNodes) {
        n.get().setV(v);
    }
    return *this;
}

void DcBus::invalidate() {
    m_valid = false;
    m_dcNodes.clear();
}

void DcBus::setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber) {
    checkValidity();
    for (const auto& n : m_dcNodes) {
        n.get().setConnectedComponentNumber(connectedComponentNumber);
    }
}

void DcBus::setDcComponentNumber(const stdcxx::optional<unsigned long>& dcComponentNumber) {
    checkValidity();
    for (auto& n : m_dcNodes) {
        n.get().setDcComponentNumber(dcComponentNumber);
    }
}

stdcxx::const_range<DcNode> DcBus::getDcNodes() const {
    checkValidity();

    const auto& mapper = stdcxx::map<std::reference_wrapper<DcNode>, DcNode>;

    return m_dcNodes | boost::adaptors::transformed(mapper);
}

stdcxx::range<DcNode> DcBus::getDcNodes() {
    checkValidity();

    const auto& mapper = stdcxx::map<std::reference_wrapper<DcNode>, DcNode>;

    return m_dcNodes | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Component> DcBus::getConnectedComponent() const {
    checkValidity();
    for (const auto& n : m_dcNodes) {
        const auto& component = n.get().getConnectedComponent();
        if(static_cast<bool>(component)) {
            return stdcxx::cref(component);
        }
    }
    throw AssertionError("Should not happen");
}

stdcxx::Reference<Component> DcBus::getConnectedComponent() {
    return stdcxx::ref(const_cast<const DcBus*>(this)->getConnectedComponent());
}

bool DcBus::isInMainConnectedComponent() const {
    const auto& cc = getConnectedComponent();
    return static_cast<bool>(cc) && (cc.get().getNum() == ComponentConstants::MAIN_NUM);

}

stdcxx::CReference<Component> DcBus::getDcComponent() const {
    checkValidity();
    for (const auto& n : m_dcNodes) {
        const auto& component = n.get().getDcComponent();
        if(static_cast<bool>(component)) {
            return stdcxx::cref(component);
        }
    }
    throw AssertionError("Should not happen");
}

stdcxx::Reference<Component> DcBus::getDcComponent() {
    return stdcxx::ref(const_cast<const DcBus*>(this)->getDcComponent());
}

unsigned long DcBus::getDcTerminalCount() const {
    checkValidity();

    unsigned long count = 0;
    for (const auto& n : m_dcNodes) {
        count += n.get().getDcTerminalCount();
    }

    return count;
}

stdcxx::const_range<DcTerminal> DcBus::getDcTerminals() const {
    checkValidity();

    const auto& mapper = [](const std::reference_wrapper<DcNode>& dcNode) {
        return dcNode.get().getDcTerminals();
    };

    return m_dcNodes | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<DcTerminal> DcBus::getDcTerminals() {
    checkValidity();

    const auto& mapper = [](const std::reference_wrapper<DcNode>& dcNode) {
        return dcNode.get().getDcTerminals();
    };

    return m_dcNodes | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

unsigned long DcBus::getConnectedDcTerminalCount() const {
    checkValidity();

    unsigned long count = 0;
    for (const auto& n : m_dcNodes) {
        count += n.get().getConnectedDcTerminalCount();
    }

    return count;
}

stdcxx::const_range<DcTerminal> DcBus::getConnectedDcTerminals() const {
    checkValidity();

    const auto& mapper = [](const std::reference_wrapper<DcNode>& dcNode) {
        return dcNode.get().getConnectedDcTerminals();
    };

    return m_dcNodes | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

stdcxx::range<DcTerminal> DcBus::getConnectedDcTerminals() {
    checkValidity();

    const auto& mapper = [](const std::reference_wrapper<DcNode>& dcNode) {
        return dcNode.get().getConnectedDcTerminals();
    };

    return m_dcNodes | boost::adaptors::transformed(mapper) | stdcxx::flattened;
}

void DcBus::visitConnectedEquipments(DcTopologyVisitor& visitor) {
    checkValidity();
    for (const auto& n : m_dcNodes) {
        n.get().visitConnectedEquipments(visitor);
    }
}

void DcBus::visitConnectedOrConnectableEquipments(DcTopologyVisitor& visitor) {
    checkValidity();
    for (const auto& n : m_dcNodes) {
        n.get().visitConnectedOrConnectableEquipments(visitor);
    }
}

}  // namespace iidm

}  // namespace powsybl
