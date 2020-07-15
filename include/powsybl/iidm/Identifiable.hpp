/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLE_HPP
#define POWSYBL_IIDM_IDENTIFIABLE_HPP

#include <string>

#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Network;

class Identifiable : public virtual Validable, public Extendable {
public:
    template <typename T>
    static bool isInstanceOf(const Identifiable& identifiable);

public: // Validable
    std::string getMessageHeader() const override;

public:
    Identifiable(const Identifiable&) = delete;

    Identifiable(Identifiable&& identifiable) noexcept;

    ~Identifiable() noexcept override = default;

    Identifiable& operator=(const Identifiable&) = delete;

    Identifiable& operator=(Identifiable&&) noexcept = delete;

    const std::string& getId() const;

    const std::string& getNameOrId() const;

    virtual const Network& getNetwork() const = 0;

    virtual Network& getNetwork() = 0;

    const std::string& getOptionalName() const;

    bool hasProperty() const;

    bool hasProperty(const std::string& key) const;

    const std::string& getProperty(const std::string& key) const;

    const std::string& getProperty(const std::string& key, const std::string& defaultValue) const;

    stdcxx::const_range<std::string> getPropertyNames() const;

    stdcxx::optional<std::string> setProperty(const std::string& key, const std::string& value);

protected:
    Identifiable(const std::string& id, const std::string& name);

private:
    virtual const std::string& getTypeDescription() const = 0;

private:
    std::string m_id;

    std::string m_name;

    stdcxx::Properties m_properties;
};

std::ostream& operator<<(std::ostream& stream, const Identifiable& identifiable);

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Identifiable.hxx>

#endif  // POWSYBL_IIDM_IDENTIFIABLE_HPP
