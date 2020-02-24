/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_FILESYSTEM_HPP
#define POWSYBL_STDCXX_FILESYSTEM_HPP

#include <string>
#include <vector>

namespace stdcxx {

std::vector<std::string> getFileList(const std::string& directory, const std::string& extension);

const std::string& sharedLibExtension();

}  // namespace stdcxx

#endif //POWSYBL_STDCXX_FILESYSTEM_HPP

