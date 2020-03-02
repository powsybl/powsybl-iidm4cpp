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

#include <boost/dll/shared_library.hpp>
#include <boost/regex.hpp>

#include <powsybl/iidm/ExtensionProvider.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename = typename std::enable_if<std::is_base_of<ExtensionProvider, T>::value>::type>
class ExtensionProviders {
public:
    static ExtensionProviders& getInstance();

    static void addExtensions(const std::string& path, const boost::regex& files);

public:
    stdcxx::CReference<T> findProvider(const std::string& name);

    const T& findProviderOrThrowException(const std::string& name);

private:
    static std::vector<std::string> getFiles(const std::string& directory, const boost::regex& file);

private:
    ExtensionProviders() = default;

    ~ExtensionProviders() noexcept;

private:
    static std::vector<boost::dll::shared_library> m_handlers;

    static std::map<std::string, std::unique_ptr<T>> m_providers;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ExtensionProviders.hxx>

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDERS_HPP
