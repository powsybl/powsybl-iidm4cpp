/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_LOADDETAIL_HPP
#define POWSYBL_IIDM_EXTENSIONS_LOADDETAIL_HPP

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>

namespace powsybl {

namespace iidm {

class Load;

namespace extensions {

class LoadDetail : public AbstractMultiVariantIdentifiableExtension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    LoadDetail(Load& load, double fixedActivePower, double fixedReactivePower, double variableActivePower, double variableReactivePower);

    ~LoadDetail() noexcept override = default;

    double getFixedActivePower() const;

    double getFixedReactivePower() const;

    double getVariableActivePower() const;

    double getVariableReactivePower() const;

    LoadDetail& setFixedActivePower(double fixedActivePower);

    LoadDetail& setFixedReactivePower(double fixedReactivePower);

    LoadDetail& setVariableActivePower(double variableActivePower);

    LoadDetail& setVariableReactivePower(double variableReactivePower);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static double checkPower(double value, const std::string& message);

private:
    std::vector<double> m_fixedActivePower;

    std::vector<double> m_fixedReactivePower;

    std::vector<double> m_variableActivePower;

    std::vector<double> m_variableReactivePower;
};

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_LOADDETAIL_HPP
