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
#include <set>
#include <string>
#include <type_traits>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <powsybl/iidm/ExtensionProvider.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename = typename std::enable_if<std::is_base_of<ExtensionProvider, T>::value>::type>
class ExtensionProviders {
public:
    static ExtensionProviders& getInstance();

    static void loadExtensions(const std::string& path, const boost::regex& files);

public:
    stdcxx::CReference<T> findProvider(const std::string& name) const;

    const T& findProviderOrThrowException(const std::string& name) const;

private:
    static std::vector<std::string> getFiles(const std::string& directory, const boost::regex& file);

    static void loadLibrary(const std::string& library);

private:
    ExtensionProviders() = default;

    ExtensionProviders(const ExtensionProviders& other) = default;

    ExtensionProviders(ExtensionProviders&& fixture) noexcept = default;

    ~ExtensionProviders() noexcept = default;

    ExtensionProviders& operator=(const ExtensionProviders& fixture) = default;

    ExtensionProviders& operator=(ExtensionProviders&& fixture) noexcept = default;

private:
    static std::set<boost::filesystem::path> m_loadedLibraries;

    static std::map<std::string, std::unique_ptr<T>> m_providers;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ExtensionProviders.hxx>

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP
