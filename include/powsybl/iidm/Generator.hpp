/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_GENERATOR_HPP
#define POWSYBL_IIDM_GENERATOR_HPP

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

class Generator : public Injection {
public:
    ~Generator() override = default;

protected:
    Generator(const std::string& id, const std::string& name);

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_GENERATOR_HPP
