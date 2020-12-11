/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/Identifiables.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

namespace Identifiables {

std::string getUniqueId(const std::string& baseId, const stdcxx::Predicate<std::string>& containsId) {
    logging::Logger& logger = logging::LoggerFactory::getLogger("Identifiables");

    std::string checkedBaseId;
    if (!baseId.empty()) {
        if (!containsId(baseId)) {
            return baseId;
        }
        checkedBaseId = baseId;
    } else {
        checkedBaseId = "autoId";
    }

    std::string uniqueId;
    unsigned long i = 0;
    do {
        uniqueId = checkedBaseId + '#' + std::to_string(i);
        ++i;
    } while (i < std::numeric_limits<unsigned long>::max() && containsId(uniqueId));

    logger.trace("Object '%1%' is not unique, rename to '%2%'", baseId, uniqueId);

    return uniqueId;
}

}  // namespace Identifiables

}  // namespace iidm

}  // namespace powsybl
