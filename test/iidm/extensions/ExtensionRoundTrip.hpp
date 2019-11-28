/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_EXTENSIONROUNDTRIP_HPP
#define POWSYBL_IIDM_EXTENSIONS_EXTENSIONROUNDTRIP_HPP

#include <memory>
#include <string>

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

class ExtensionRoundTrip {
public:
    using Comparator = std::function<void(const std::string&, const std::string&)>;

    using Reader = std::function<std::unique_ptr<Extension>(const std::string&)>;

    using Writer = std::function<void(const Extension&, std::ostream&)>;

public:
    static std::unique_ptr<Extension> runXml(Extendable& extendable, const Extension& extension, const converter::xml::ExtensionXmlSerializer& serializer, const std::string& ref);

private:
    static std::string write(const Extension& extension, const Writer& out, const Comparator& compare, const std::string& ref);
};

}

}

}

#endif  // POWSYBL_IIDM_EXTENSIONS_EXTENSIONROUNDTRIP_HPP
