/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSION_HXX
#define POWSYBL_IIDM_EXTENSION_HXX

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/stdcxx/MessageFormat.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

template <typename E, typename, typename... Args>
std::unique_ptr<Extension> Extension::create(Args&&... args) {
    return stdcxx::make_unique<Extension, E>(std::forward<Args>(args)...);
}

template <typename E, typename>
stdcxx::CReference<E> Extension::getExtendable() const {
    if (m_extendable && !stdcxx::isInstanceOf<E>(m_extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle<E>(), stdcxx::demangle(m_extendable.get())));
    }
    return stdcxx::cref<E>(m_extendable);
}

template <typename E, typename>
stdcxx::Reference<E> Extension::getExtendable() {
    if (m_extendable && !stdcxx::isInstanceOf<E>(m_extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle<E>(), stdcxx::demangle(m_extendable.get())));
    }
    return stdcxx::ref<E>(m_extendable);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSION_HXX
