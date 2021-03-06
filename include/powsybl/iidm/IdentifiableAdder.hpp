/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLEADDER_HPP
#define POWSYBL_IIDM_IDENTIFIABLEADDER_HPP

#include <string>

#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class Network;

template<typename Adder>
class IdentifiableAdder : public Validable {
public: // Validable
    std::string getMessageHeader() const override;

public:
    IdentifiableAdder() = default;

    ~IdentifiableAdder() noexcept override = default;

    Adder& setEnsureIdUnicity(bool ensureIdUnicity);

    virtual Adder& setFictitious(bool fictitious);

    virtual Adder& setId(const std::string& id);

    virtual Adder& setName(const std::string& name);

protected:
    std::string checkAndGetUniqueId() const;

    const std::string& getName() const;

    virtual const Network& getNetwork() const = 0;

    virtual Network& getNetwork() = 0;

    bool isFictitious() const;

private:
    virtual const std::string& getTypeDescription() const = 0;

private:
    std::string m_id;

    bool m_ensureIdUnicity = false;

    std::string m_name;

    bool m_fictitious = false;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/IdentifiableAdder.hxx>

#endif  // POWSYBL_IIDM_IDENTIFIABLEADDER_HPP
