/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX
#define POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX

#include <powsybl/iidm/ExtensionProviders.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename Dummy>
ExtensionProviders<T, Dummy>& ExtensionProviders<T, Dummy>::getInstance() {
    static ExtensionProviders<T, Dummy> s_instance;
    return s_instance;
}

template <typename T, typename Dummy>
stdcxx::CReference<T> ExtensionProviders<T, Dummy>::findProvider(const std::string& name) const {
    stdcxx::CReference<T> provider;

    const auto& it = m_providers.find(name);
    if (it != m_providers.end()) {
        provider = stdcxx::cref(it->second);
    }

    return provider;

}

template <typename T, typename Dummy>
const T& ExtensionProviders<T, Dummy>::findProviderOrThrowException(const std::string& name) const {
    const auto& it = m_providers.find(name);
    if (it == m_providers.end()) {
        throw PowsyblException(logging::format("No provider found for extension '%1'", name));
    }
    return *it->second;
}

template <typename T, typename Dummy>
void ExtensionProviders<T, Dummy>::registerExtension(const std::string& name, std::unique_ptr<T>&& provider) {
    getInstance().m_providers.emplace(std::make_pair(name, std::move(provider)));
}


}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HXX
