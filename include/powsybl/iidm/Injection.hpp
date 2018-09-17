/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INJECTION_HPP
#define POWSYBL_IIDM_INJECTION_HPP

#include <functional>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

class Injection : public Connectable {
public:
    ~Injection() noexcept override = default;

    const Terminal& getTerminal() const;

    Terminal& getTerminal();

protected: // Connectable
    using Connectable::getTerminal;

protected:
    Injection(const std::string& id, const std::string& name, const ConnectableType& connectableType);
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_INJECTION_HPP
