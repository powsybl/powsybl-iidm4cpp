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
#include <string>
#include <type_traits>

#include <boost/dll/shared_library.hpp>
#include <boost/filesystem.hpp>

#include <powsybl/iidm/ExtensionProvider.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename = typename std::enable_if<std::is_base_of<ExtensionProvider, T>::value>::type>
class ExtensionProviders {
public:
    static ExtensionProviders& getInstance();

public:
    ExtensionProviders(const ExtensionProviders&) = delete;

    ExtensionProviders(ExtensionProviders&&) noexcept = delete;

    ExtensionProviders& operator=(const ExtensionProviders&) = delete;

    ExtensionProviders& operator=(ExtensionProviders&&) noexcept = delete;

    stdcxx::CReference<T> findProvider(const std::string& name) const;

    const T& findProviderOrThrowException(const std::string& name) const;

    stdcxx::const_range<T> getProviders() const;

    stdcxx::range<T> getProviders();

    void loadExtensions(const boost::filesystem::path& directory, const std::regex& pattern);

private:
    ExtensionProviders();

    ~ExtensionProviders() noexcept = default;

    const std::string& getSymbolName() const;

    void loadLibrary(const boost::filesystem::path& libraryPath);

    void registerExtension(std::unique_ptr<T>&& provider, const boost::filesystem::path& libraryPath = boost::filesystem::path());

private:
    std::map<boost::filesystem::path, boost::dll::shared_library> m_loadedLibraries;

    std::map<std::string, std::unique_ptr<T>> m_providers;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP
