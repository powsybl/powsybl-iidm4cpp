/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OVERLOAD_HPP
#define POWSYBL_IIDM_OVERLOAD_HPP

#include <string>

#include <powsybl/iidm/LoadingLimits.hpp>

namespace powsybl {

namespace iidm {

class Overload {

public:
    /**
    * encapsulated "Unacceptable" fictitious limit of acceptable duration 0 and infinite value
    * internally used for Overload without acceptable temporary limit.
    * PropertiesHolder capabilities inherited through TemporaryLimit overriden to an "unsupported" behavior.
    */
    class UnacceptableTemporaryLimit : public LoadingLimits::TemporaryLimit {
        private:
        UnacceptableTemporaryLimit() : LoadingLimits::TemporaryLimit("Unacceptable", std::numeric_limits<double>::infinity(), 0UL, true) {};
        friend class Overload; //only encapsulating Overload class can build UnacceptableTemporaryLimit

        public:
        ~UnacceptableTemporaryLimit() noexcept = default;

        //Override PropertiesHolder methods for unsupported behavior
        bool hasProperty() const override;

        bool hasProperty(const std::string& key) const override;

        const std::string& getProperty(const std::string& key) const override;

        const std::string& getProperty(const std::string& key, const std::string& defaultValue) const override;

        stdcxx::optional<std::string> setProperty(const std::string& key, const std::string& value) override;

        bool removeProperty(const std::string& key) override;

        stdcxx::const_range<std::string> getPropertyNames() const override;

        void copyPropertiesTo(PropertiesHolder& propertiesHolder) const override;
    };

    Overload(const LoadingLimits::TemporaryLimit &temporaryLimit, const std::string& operationalLimitsGroupId, const std::string &previousLimitName, double previousLimit, double limitReductionCoefficient = 1.0);

    Overload(const std::string& operationalLimitsGroupId, const std::string &previousLimitName, double previousLimit, double limitReductionCoefficient = 1.0);

    ~Overload() noexcept = default;

    double getPreviousLimit() const;

    const std::string &getPreviousLimitName() const;

    const LoadingLimits::TemporaryLimit &getTemporaryLimit() const;

    double getLimitReductionCoefficient() const;

    const std::string& getOperationalLimitsGroupId() const;

private:

    std::unique_ptr<LoadingLimits::TemporaryLimit> m_temporaryLimit;

    std::string m_operationLimitsGroupId;

    std::string m_previousLimitName;

    double m_previousLimit;

    double m_limitReductionCoefficient;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OVERLOAD_HPP
