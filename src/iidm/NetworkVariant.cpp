/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkVariant.hpp>

#include <powsybl/iidm/ConnectedComponentsManager.hpp>
#include <powsybl/iidm/SynchronousComponentsManager.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace network {

VariantImpl::VariantImpl(Network& network) :
    Variant(network),
    m_connectedComponentsManager(network),
    m_synchronousComponentsManager(network) {
}

VariantImpl::VariantImpl(Network& network, VariantImpl&& variant) noexcept :
    Variant(network),
    m_connectedComponentsManager(network, std::move(variant.m_connectedComponentsManager)),
    m_synchronousComponentsManager(network, std::move(variant.m_synchronousComponentsManager)) {
}

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>(m_owner);
}

const ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() const {
    return m_connectedComponentsManager;
}

ConnectedComponentsManager& VariantImpl::getConnectedComponentsManager() {
    return m_connectedComponentsManager;
}

const SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() const {
    return m_synchronousComponentsManager;
}

SynchronousComponentsManager& VariantImpl::getSynchronousComponentsManager() {
    return m_synchronousComponentsManager;
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
