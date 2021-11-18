/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTS_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTS_HPP

#include <string>
#include <vector>

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace extensions {

namespace iidm {

class Measurement;
class MeasurementAdder;
class MeasurementsAdder;

class Measurements : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~Measurements() noexcept override = default;

    void add(std::unique_ptr<Measurement>&& measurement);

    void cleanIfEmpty();

    stdcxx::CReference<Measurement> getMeasurement(const std::string& id) const;

    stdcxx::Reference<Measurement> getMeasurement(const std::string& id);

    stdcxx::const_range<Measurement> getMeasurements() const;

    stdcxx::const_range<Measurement> getMeasurements(const Measurement::Type& type) const;

    stdcxx::range<Measurement> getMeasurements(const Measurement::Type& type);

    MeasurementAdder newMeasurement();

    void remove(const Measurement& measurement);

protected:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    explicit Measurements(Identifiable& identifiable);

    friend class MeasurementsAdder;

private:
    std::vector<std::unique_ptr<Measurement>> m_measurements;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTS_HPP
