/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTS_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTS_HPP

#include <list>

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurement.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace extensions {

namespace iidm {

class DiscreteMeasurementAdder;
class DiscreteMeasurementsAdder;

class DiscreteMeasurements : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    DiscreteMeasurements& add(std::unique_ptr<DiscreteMeasurement>&& discreteMeasurement);

    void cleanIfEmpty();

    stdcxx::CReference<DiscreteMeasurement> getDiscreteMeasurement(const std::string& id) const;

    stdcxx::Reference<DiscreteMeasurement> getDiscreteMeasurement(const std::string& id);

    stdcxx::const_range<DiscreteMeasurement> getDiscreteMeasurements() const;

    stdcxx::range<DiscreteMeasurement> getDiscreteMeasurements();

    stdcxx::const_range<DiscreteMeasurement> getDiscreteMeasurements(const DiscreteMeasurement::Type& type) const;

    stdcxx::range<DiscreteMeasurement> getDiscreteMeasurements(const DiscreteMeasurement::Type& type);

    DiscreteMeasurementAdder newDiscreteMeasurement();

    void remove(const DiscreteMeasurement& discreteMeasurement);

protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    explicit DiscreteMeasurements(Identifiable& identifiable);

    friend class DiscreteMeasurementsAdder;

private:
    std::list<std::unique_ptr<DiscreteMeasurement>> m_discreteMeasurements;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTS_HPP
