/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP
#define POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP

#include <set>
#include <string>

namespace powsybl {

namespace iidm {

namespace converter {

class ImportOptions {
public:
    ImportOptions() = default;

    ImportOptions& addExtension(const std::string& extension);

    bool isThrowExceptionIfExtensionNotFound() const;

    ImportOptions& setExtensions(const std::set<std::string>& extensions);

    ImportOptions& setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound);

    bool withExtension(const std::string& extension) const;

private:
    bool m_throwExceptionIfExtensionNotFound{false};

    std::set<std::string> m_extensions;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP
