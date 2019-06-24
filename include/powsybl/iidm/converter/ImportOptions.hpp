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

#include <boost/optional.hpp>

namespace powsybl {

namespace iidm {

class ImportOptions {
public:
    ImportOptions() = default;

    bool isThrowExceptionIfExtensionNotFound() const;

    ImportOptions& setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound);

private:
    bool m_throwExceptionIfExtensionNotFound{false};
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_IMPORTOPTIONS_HPP
