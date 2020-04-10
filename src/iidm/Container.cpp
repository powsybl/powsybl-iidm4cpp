/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Container.hpp>

namespace powsybl {

namespace iidm {

Container::Container(const std::string& id, const std::string& name, const Container::Type& type) :
    Identifiable(id, name),
    m_type(type) {
}

Container::Container(Container&& container) noexcept :
    Identifiable(std::move(container)),
    m_type(container.m_type) {
}

const Container::Type& Container::getContainerType() const {
    return m_type;
}

}  // namespace iidm

}  // namespace powsybl
