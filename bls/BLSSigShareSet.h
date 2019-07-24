/*
    Copyright (C) 2018-2019 SKALE Labs

  This file is part of libBLS.

  libBLS is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  libBLS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with libBLS.  If not, see <https://www.gnu.org/licenses/>.

    @file BLSSigShare.h
    @author Stan Kladko
    @date 2019
*/

#ifndef LIBBLS_BLSSIGSHARESET_H
#define LIBBLS_BLSSIGSHARESET_H


#include <stdlib.h>
#include <string>
#include "bls.h"

class BLSSignature;

class BLSSigShareSet {
    bool was_merged;

    size_t totalSigners;
    size_t requiredSigners;

    std::map<size_t, std::shared_ptr< BLSSigShare > > sigShares;

public:

    BLSSigShareSet( size_t requiredSigners, size_t totalSigners );

    bool isEnough();

    bool addSigShare( std::shared_ptr< BLSSigShare > _sigShare);

    unsigned long getTotalSigSharesCount();
    std::shared_ptr< BLSSigShare > getSigShareByIndex(size_t _index);
    std::shared_ptr<BLSSignature> merge();

    //std::map<size_t, std::shared_ptr< BLSSigShare > > getSigShares();
};



#endif  // LIBBLS_BLSSIGSHARESET_H-
