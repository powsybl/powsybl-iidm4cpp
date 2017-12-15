/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLE_HPP
#define POWSYBL_IIDM_IDENTIFIABLE_HPP

#include <string>

#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class Identifiable : public Validable {
public:
    std::string getMessageHeader() const override;

public:
    virtual ~Identifiable() = default;

    const std::string& getId() const;

    const std::string& getName() const;

protected:
    virtual const std::string& getTypeDescription() const = 0;

protected:
    Identifiable(Identifiable&& identifiable) = default;

    Identifiable(const std::string& id, const std::string& name);

private: // Non copyable
    Identifiable(const Identifiable& identifiable) = delete;

    Identifiable& operator=(const Identifiable& identifiable) = delete;

private:
    std::string m_id;

    std::string m_name;
};

}

}

#endif  // POWSYBL_IIDM_IDENTIFIABLE_HPP
