/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Identifiable.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Identifiable::Identifiable(const std::string& id, const std::string& name) :
    m_id(checkNotEmpty(id, "Invalid id")),
    m_name(name) {
}

std::string Identifiable::getMessageHeader() const {
    return getTypeDescription() + " '" + m_id + "': ";
}

const std::string& Identifiable::getId() const {
    return m_id;
}

const std::string& Identifiable::getName() const {
    return m_name;
}

}

}
