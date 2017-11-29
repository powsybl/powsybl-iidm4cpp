/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ValidationException.hpp>

namespace powsybl {

namespace iidm {

ValidationException::ValidationException(const Validable& validable, const std::string& message) :
    PowsyblException(validable.getMessageHeader() + message) {
}

}

}
