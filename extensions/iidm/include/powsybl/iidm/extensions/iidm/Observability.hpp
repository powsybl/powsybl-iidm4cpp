/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_OBSERVABILITY_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_OBSERVABILITY_HPP

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class Observability {
public:
    bool isObservable() const;

    Observability& setObservable(bool observable);

protected:
    explicit Observability(bool observable);

private:
    bool m_isObservable;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_OBSERVABILITY_HPP
