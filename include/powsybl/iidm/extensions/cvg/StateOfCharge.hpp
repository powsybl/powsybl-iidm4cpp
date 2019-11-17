/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_STATEOFCHARGE_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_STATEOFCHARGE_HPP

#include <vector>

#include <powsybl/iidm/AbstractMultiVariantConnectableExtension.hpp>
#include <powsybl/iidm/Battery.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

class StateOfCharge : public AbstractMultiVariantConnectableExtension {
public:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    StateOfCharge(Battery& battery, double storageCapacity, double current);

    StateOfCharge(Battery& battery, double min, double max, double storageCapacity, double current);

    double getCurrent() const;

    double getCurrentEnergy() const;

    double getMax() const;

    double getMaxEnergy() const;

    double getMin() const;

    double getMinEnergy() const;

    double getStorageCapacity() const;

    StateOfCharge& setCurrent(double current);

    StateOfCharge& setMax(double max);

    StateOfCharge& setMin(double min);

    StateOfCharge& setStorageCapacity(double storageCapacity);

private:
    static void checkBound(double min, double max);

    static double checkCurrent(double current, double min, double max);

    static double checkMax(double max);

    static double checkMin(double min);

    static double checkStorageCapacity(double storageCapacity);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::vector<double> m_current;

    double m_min;

    double m_max;

    double m_storageCapacity;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_STATEOFCHARGE_HPP

