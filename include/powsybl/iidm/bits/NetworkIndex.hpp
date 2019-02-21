/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BITS_NETWORKINDEX_HPP
#define POWSYBL_IIDM_BITS_NETWORKINDEX_HPP

#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace powsybl {

namespace iidm {

class Identifiable;

typedef std::unordered_map<std::string, std::unique_ptr<Identifiable> > IdentifiableById;

typedef std::vector<std::reference_wrapper<Identifiable> > Identifiables;

typedef std::unordered_map<std::type_index, Identifiables> IdentifiablesByType;

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BITS_NETWORKINDEX_HPP
