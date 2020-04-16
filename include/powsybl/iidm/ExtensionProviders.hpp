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
#include <regex>
#include <set>
#include <string>
#include <type_traits>

#include <boost/filesystem.hpp>

#include <powsybl/iidm/ExtensionProvider.hpp>
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename = typename std::enable_if<std::is_base_of<ExtensionProvider, T>::value>::type>
class IIDM_DECLSPEC ExtensionProviders {
public:
    static ExtensionProviders& getInstance();

public:
    ExtensionProviders(const ExtensionProviders&) = delete;

    ExtensionProviders(ExtensionProviders&&) = delete;

    ExtensionProviders& operator=(const ExtensionProviders&) = delete;

    ExtensionProviders& operator=(ExtensionProviders&&) = delete;

    stdcxx::CReference<T> findProvider(const std::string& name) const;

    const T& findProviderOrThrowException(const std::string& name) const;

    void loadExtensions(const boost::filesystem::path& directory, const std::regex& pattern);

private:
    ExtensionProviders() = default;

    ~ExtensionProviders() noexcept = default;

    void loadLibrary(const boost::filesystem::path& libraryPath);

private:
    std::set<boost::filesystem::path> m_loadedLibraries;

    std::map<std::string, std::unique_ptr<T>> m_providers;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP
