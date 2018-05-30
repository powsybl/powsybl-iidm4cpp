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
public: // Validable
    std::string getMessageHeader() const override;

public:
    ~Identifiable() override = default;

    const std::string& getId() const;

    const std::string& getName() const;

protected:
    Identifiable(Identifiable&&) = default;

    Identifiable(const std::string& id, const std::string& name);

private:
    virtual const std::string& getTypeDescription() const = 0;

private: // Non copyable
    Identifiable(const Identifiable& identifiable) = delete;

    Identifiable& operator=(const Identifiable& identifiable) = delete;

private:
    std::string m_id;

    std::string m_name;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_IDENTIFIABLE_HPP
