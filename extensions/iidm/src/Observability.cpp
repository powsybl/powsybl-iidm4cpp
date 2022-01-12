/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/Observability.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

Observability::Observability(bool observable) :
    m_isObservable(observable) {
}

bool Observability::isObservable() const {
    return m_isObservable;
}

Observability& Observability::setObservable(bool observable) {
    m_isObservable = observable;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
