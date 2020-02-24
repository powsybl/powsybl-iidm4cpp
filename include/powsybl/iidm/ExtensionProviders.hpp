/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP
#define POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <powsybl/iidm/ExtensionProvider.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename = typename std::enable_if<std::is_base_of<ExtensionProvider, T>::value>::type>
class ExtensionProviders {
public:
    static void clear();

    static void initialize(const std::string& extensionsPath);

    static void initialize(const std::vector<std::string>& searchPaths);

    static void registerExtension(const std::string& name, std::unique_ptr<T>&& provider);

public:
    static stdcxx::CReference<T> findProvider(const std::string& name);

    static const T& findProviderOrThrowException(const std::string& name);

private:
    static std::map<std::string, std::unique_ptr<T>> m_providers;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ExtensionProviders.hxx>

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP
