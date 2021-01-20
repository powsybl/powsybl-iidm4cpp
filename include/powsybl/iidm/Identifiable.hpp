/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLE_HPP
#define POWSYBL_IIDM_IDENTIFIABLE_HPP

#include <set>
#include <string>

#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Network;

class Identifiable : public virtual Validable, public Extendable, public MultiVariantObject {
public: // Validable
    std::string getMessageHeader() const override;

public:
    Identifiable(const Identifiable&) = delete;

    // Move constructor of stdcxx::Properties is not marked noexcept
    Identifiable(Identifiable&&) = default;  // NOSONAR

    ~Identifiable() noexcept override = default;

    Identifiable& operator=(const Identifiable&) = delete;

    Identifiable& operator=(Identifiable&&) noexcept = delete;

    void addAlias(const std::string& alias);

    const std::set<std::string>& getAliases() const;

    const std::string& getId() const;

    const std::string& getNameOrId() const;

    virtual const Network& getNetwork() const = 0;

    virtual Network& getNetwork() = 0;

    const std::string& getOptionalName() const;

    const std::string& getProperty(const std::string& key) const;

    const std::string& getProperty(const std::string& key, const std::string& defaultValue) const;

    stdcxx::const_range<std::string> getPropertyNames() const;

    bool hasAliases() const;

    bool hasProperty() const;

    bool hasProperty(const std::string& key) const;

    bool isFictitious() const;

    void removeAlias(const std::string& alias);

    virtual void setFictitious(bool fictitious);

    stdcxx::optional<std::string> setProperty(const std::string& key, const std::string& value);

protected:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    Identifiable(const std::string& id, const std::string& name, bool fictitious);

private:
    virtual const std::string& getTypeDescription() const = 0;

private:
    std::string m_id;

    std::string m_name;

    bool m_fictitious;

    stdcxx::Properties m_properties;

    std::set<std::string> m_aliases;
};

std::ostream& operator<<(std::ostream& stream, const Identifiable& identifiable);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_IDENTIFIABLE_HPP
