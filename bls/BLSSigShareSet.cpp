//
// Created by kladko on 7/5/19.
//

#include <stdint.h>
#include <string>

using namespace std;


#include "BLSSignature.h"
#include "BLSSigShare.h"
#include "BLSSigShareSet.h"
#include "BLSutils.h"


bool BLSSigShareSet::addSigShare( shared_ptr< BLSSigShare > _sigShare ) {

    if ( was_merged )
        BOOST_THROW_EXCEPTION(runtime_error("Invalid state"));

    if ( !_sigShare ) {
        BOOST_THROW_EXCEPTION( runtime_error( "Null _sigShare" ) );
    }

    if ( sigShares.count( _sigShare->getSignerIndex() ) > 0 ) {
        BOOST_THROW_EXCEPTION( runtime_error(
            "Already have this index:" + to_string( _sigShare->getSignerIndex() ) ) );
        return false;
    }
    sigShares[_sigShare->getSignerIndex()] = _sigShare;

    return true;
}

size_t BLSSigShareSet::getTotalSigSharesCount() {

    return sigShares.size();
}
shared_ptr< BLSSigShare > BLSSigShareSet::getSigShareByIndex( size_t _index ) {

    if ( _index == 0 || _index > totalSigners ) {
        BOOST_THROW_EXCEPTION( runtime_error( "Index out of range:" + to_string( _index ) ) );
    }


    if ( sigShares.count( _index ) == 0 ) {
        return nullptr;
    }

    return sigShares.at( _index );
}
BLSSigShareSet::BLSSigShareSet( size_t _requiredSigners, size_t _totalSigners )
    : requiredSigners( _requiredSigners ), totalSigners( _totalSigners ), was_merged(false) {
    BLSSignature::checkSigners( _requiredSigners, _totalSigners );
}
bool BLSSigShareSet::isEnough() {

    return ( sigShares.size() >= requiredSigners );
}


shared_ptr< BLSSignature > BLSSigShareSet::merge() {

    if (!isEnough())
        BOOST_THROW_EXCEPTION(runtime_error("Not enough shares to create signature"));

    was_merged = true;
    signatures::Bls obj = signatures::Bls( requiredSigners, totalSigners );

    vector< size_t > participatingNodes;
    vector< libff::alt_bn128_G1 > shares;

    for ( auto&& item : sigShares ) {
        participatingNodes.push_back( static_cast< uint64_t >( item.first ) );
        shares.push_back( *item.second->getSigShare() );
    }

    vector< libff::alt_bn128_Fr > lagrangeCoeffs = obj.LagrangeCoeffs( participatingNodes );

    libff::alt_bn128_G1 signature = obj.SignatureRecover( shares, lagrangeCoeffs );

    auto sigPtr = make_shared< libff::alt_bn128_G1 >( signature );

    return make_shared< BLSSignature >( sigPtr, requiredSigners, totalSigners );
}


/*std::map<size_t, std::shared_ptr< BLSSigShare > > BLSSigShareSet::getSigShares(){
    return sigShares;
}*/