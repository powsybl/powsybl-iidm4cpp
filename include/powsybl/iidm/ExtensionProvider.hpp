/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONPROVIDER_HPP
#define POWSYBL_IIDM_EXTENSIONPROVIDER_HPP

#include <string>

#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class IIDM_DECLSPEC ExtensionProvider {
public:
    virtual ~ExtensionProvider() noexcept = default;

    const std::string& getCategoryName() const;

    const std::string& getExtensionName() const;

protected:
    ExtensionProvider(const std::string& extensionName, const std::string& categoryName);

private:
    std::string m_extensionName;

    std::string m_categoryName;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDER_HPP
