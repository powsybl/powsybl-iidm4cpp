/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROL_HPP

#include <powsybl/iidm/AbstractMultiVariantIdentifiableExtension.hpp>

namespace powsybl {

namespace iidm {

class Generator;

namespace extensions {

namespace iidm {

class CoordinatedReactiveControl : public AbstractMultiVariantIdentifiableExtension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    ~CoordinatedReactiveControl() noexcept override = default;

    double getQPercent() const;

    CoordinatedReactiveControl& setQPercent(double qPercent);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static double checkQPercent(const Generator& generator, double qPercent);

private:
    CoordinatedReactiveControl(Generator& generator, double qPercent);

    friend class CoordinatedReactiveControlAdder;

private:
    std::vector<double> m_qPercent;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATEDREACTIVECONTROL_HPP

