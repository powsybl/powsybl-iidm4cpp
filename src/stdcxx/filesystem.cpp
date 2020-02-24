/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/filesystem.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace stdcxx {

std::vector<std::string> getFileList(const std::string& directory, const std::string& extension) {
    std::vector<std::string> libs;
    if (boost::filesystem::exists(directory) && boost::filesystem::is_directory(directory)) {
        boost::filesystem::recursive_directory_iterator it(directory);
        boost::filesystem::recursive_directory_iterator endit;

        while (it != endit)
        {
            if (boost::filesystem::is_regular_file(*it) && it->path().extension() == extension) {
                libs.push_back((directory / it->path().filename()).string());
            }
            ++it;
        }
    }
    return libs;
}

const std::string& sharedLibExtension() {
#if defined __GNUC__
    static std::string s_extension = ".so";
#elif defined _WIN64
#error "To be updated"
#endif
    return s_extension;
}

}  // namespace stdcxx

