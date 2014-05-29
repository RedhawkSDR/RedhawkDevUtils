/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK Basic Components RedhawkDevUtils.
 *
 * REDHAWK Basic Components RedhawkDevUtils is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK Basic Components RedhawkDevUtils is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.

 * This is the component code. This file contains the child class where
 * custom functionality can be added to the component. Custom
 * functionality to the base class can be extended here. Access to
 * the ports can also be done from this class
 */
#ifndef RH_UTILS_H
#define RH_UTILS_H

#include <vector>
#include <queue>
#include <complex>
#include <set>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <uuid/uuid.h>
#include <omniORB4/CORBA.h>
#include <omniORB4/omniURI.h>
#include <omniORB4/omniORB.h>
#include <ossie/Resource_impl.h>
#include <ossie/prop_helpers.h>
#include <ossie/Resource_impl.h>
#include <ossie/CorbaUtils.h>
#include <omniORB4/CORBA.h>
#include <omniORB4/omniURI.h>
#include <omniORB4/omniORB.h>
#include <BULKIO/bulkioDataTypes.h>
#include <boost/thread.hpp>
#include <boost/thread/detail/thread_group.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

/*********************************************************************************************/
/**************************             UUID_HELPER                 **************************/
/*********************************************************************************************/
namespace UUID_HELPER {
	inline  std::string new_uuid(  ) {
		uuid_t  new_random_uuid;
		uuid_generate_random( new_random_uuid );
		char    new_random_uuid_str[37];
		uuid_unparse( new_random_uuid, new_random_uuid_str );
		return std::string( new_random_uuid_str );
	}
}
using namespace UUID_HELPER;

/*********************************************************************************************/
/**************************            BASE64                **************************/
/*********************************************************************************************/
namespace BASE64_HELPER {

	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";


	static inline bool is_base64( unsigned char c ) {
		return ( isalnum( c ) || ( c == '+' ) || ( c == '/' ) );
	}
	inline std::string base64_encode( unsigned char const *bytes_to_encode, unsigned int in_len ) {
		std::string ret;
		int     i = 0;
		int     j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while( in_len-- ) {
			char_array_3[i++] = *( bytes_to_encode++ );
			if( i == 3 ) {
				char_array_4[0] = ( char_array_3[0] & 0xfc ) >> 2;
				char_array_4[1] = ( ( char_array_3[0] & 0x03 ) << 4 ) + ( ( char_array_3[1] & 0xf0 ) >> 4 );
				char_array_4[2] = ( ( char_array_3[1] & 0x0f ) << 2 ) + ( ( char_array_3[2] & 0xc0 ) >> 6 );
				char_array_4[3] = char_array_3[2] & 0x3f;

				for( i = 0; ( i < 4 ); i++ )
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if( i ) {
			for( j = i; j < 3; j++ )
				char_array_3[j] = '\0';

			char_array_4[0] = ( char_array_3[0] & 0xfc ) >> 2;
			char_array_4[1] = ( ( char_array_3[0] & 0x03 ) << 4 ) + ( ( char_array_3[1] & 0xf0 ) >> 4 );
			char_array_4[2] = ( ( char_array_3[1] & 0x0f ) << 2 ) + ( ( char_array_3[2] & 0xc0 ) >> 6 );
			char_array_4[3] = char_array_3[2] & 0x3f;

			for( j = 0; ( j < i + 1 ); j++ )
				ret += base64_chars[char_array_4[j]];

			while( ( i++ < 3 ) )
				ret += '=';

		}

		return ret;

	}
	inline  std::string base64_decode( std::string const &encoded_string ) {
		int     in_len = encoded_string.size(  );
		int     i = 0;
		int     j = 0;
		int     in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while( in_len-- && ( encoded_string[in_] != '=' ) && is_base64( encoded_string[in_] ) ) {
			char_array_4[i++] = encoded_string[in_];
			in_++;
			if( i == 4 ) {
				for( i = 0; i < 4; i++ )
					char_array_4[i] = base64_chars.find( char_array_4[i] );

				char_array_3[0] = ( char_array_4[0] << 2 ) + ( ( char_array_4[1] & 0x30 ) >> 4 );
				char_array_3[1] = ( ( char_array_4[1] & 0xf ) << 4 ) + ( ( char_array_4[2] & 0x3c ) >> 2 );
				char_array_3[2] = ( ( char_array_4[2] & 0x3 ) << 6 ) + char_array_4[3];

				for( i = 0; ( i < 3 ); i++ )
					ret += char_array_3[i];
				i = 0;
			}
		}

		if( i ) {
			for( j = i; j < 4; j++ )
				char_array_4[j] = 0;

			for( j = 0; j < 4; j++ )
				char_array_4[j] = base64_chars.find( char_array_4[j] );

			char_array_3[0] = ( char_array_4[0] << 2 ) + ( ( char_array_4[1] & 0x30 ) >> 4 );
			char_array_3[1] = ( ( char_array_4[1] & 0xf ) << 4 ) + ( ( char_array_4[2] & 0x3c ) >> 2 );
			char_array_3[2] = ( ( char_array_4[2] & 0x3 ) << 6 ) + char_array_4[3];

			for( j = 0; ( j < i - 1 ); j++ )
				ret += char_array_3[j];
		}

		return ret;
	}

}

using namespace BASE64_HELPER;


/*********************************************************************************************/
/**************************     CORBA SERIALIZATION HELPER          **************************/
/*********************************************************************************************/
namespace CORBA_SERIALIZATION_HELPER {


	inline bool any_to_serialize_string( const CORBA::Any & _any, std::string & serString ) {
		serString.clear(  );
		cdrEncapsulationStream a;
		_any >>= a;
		serString = base64_encode( ( const unsigned char * )a.bufPtr(  ), a.bufSize(  ) );
		return true;
	} inline bool serialize_string_to_any( const std::string & serString, CORBA::Any & serAny ) {
		if( serString.empty(  ) )
			return false;
		std::string tmpString = base64_decode( serString );
		cdrEncapsulationStream b((CORBA::Octet*)&tmpString[0], tmpString.size(  ), false);
		serAny <<= b;
		return true;
	}

	template < class DT > inline bool deserialize_any( const CORBA::Any & _any, std::string & _serString, DT & _serObject ) {
		try {
			DT     *dt_ptr;
			if( _any >>= _serString ) {
				if( _serString.empty(  ) )
					return true;
				CORBA::Any tmpAny;
				if( !serialize_string_to_any( _serString, tmpAny ) )
					return false;
				tmpAny >>= dt_ptr;
				_serObject = *dt_ptr;
			} else if( _any >>= dt_ptr ) {
				_serObject = *dt_ptr;
				if( !any_to_serialize_string( _any, _serString ) )
					return false;
			} else
				return false;

		}
		catch( ... ) {
			return false;
		}
		return true;
	}


}

using namespace CORBA_SERIALIZATION_HELPER;


/*********************************************************************************************/
/**************************                CORBA HELPER             **************************/
/*********************************************************************************************/
namespace CF_HELPER {

	inline void append_cf_props( CF::Properties & main_props, const CF::Properties & additional_props ) {
		size_t  curSize = main_props.length(  );
		main_props.length( curSize + additional_props.length(  ) );
		for     ( size_t i = 0; i < additional_props.length(  ); i++ )
			main_props[curSize + i] = additional_props[i];
	}

	inline void merge_cf_props(CF::Properties & main_props, const CF::Properties & additional_props, bool override_if_found = true){
		size_t curSize;
		bool found;
		for ( size_t i = 0; i < additional_props.length(  ); i++ ){
			found = false;
			curSize = main_props.length();
			for(size_t j = 0; j < curSize; j++){
				if(!strcmp(additional_props[i].id, main_props[j].id)){
					if(override_if_found)
						main_props[j] = additional_props[i];
					found = true;
					break;
				}
			}
			if(found)
				continue;
			main_props.length(curSize+1);
			main_props[curSize] = additional_props[i];
		}
	}

};


	/*********************************************************************************************/
	/**************************             BULKIO HELPERS              **************************/
	/*********************************************************************************************/
namespace BIO_HELPER {

	inline void zeroSRI( BULKIO::StreamSRI * sri ) {
		sri->hversion = 1;
		sri->xstart = 0.0;
		sri->xdelta = 1.0;
		sri->xunits = 1;
		sri->subsize = 1;
		sri->ystart = 0.0;
		sri->ydelta = 1.0;
		sri->yunits = 1;
		sri->mode = 0;
		sri->streamID = "";
		sri->keywords.length( 0 );
	};


	inline  std::string time_to_string( const BULKIO::PrecisionUTCTime & timeTag, bool include_fractional = true, bool compressed = false ) {
		time_t  _fileStartTime;
		struct tm *local;
		char    timeArray[30];
		_fileStartTime = ( time_t ) timeTag.twsec;
		local = gmtime( &_fileStartTime );      //converts second since epoch to tm struct
		if( compressed )
			strftime( timeArray, 30, "%d%b%Y.%H%M%S", local );  //prints out string from tm struct
		else
			strftime( timeArray, 30, "%d-%b-%Y %H:%M:%S", local );      //prints out string from tm struct
		std::string time = std::string( timeArray );
		if( include_fractional ) {
			char    fractSec[30];
			sprintf( fractSec, "%010.0f", timeTag.tfsec * 1e10 );
			time += "." + std::string( fractSec );
		}
		return time;
	}

	inline void zeroTime( BULKIO::PrecisionUTCTime * timeTag ) {
		timeTag->tcmode = 1;
		timeTag->tcstatus = 0;
		timeTag->toff = 0.0;
		timeTag->twsec = 0.0;
		timeTag->tfsec = 0.0;
	};

    inline void zeroTime(BULKIO::PrecisionUTCTime& timeTag) {
        BIO_HELPER::zeroTime(&timeTag);
    };


	inline  BULKIO::PrecisionUTCTime getSystemTimestamp( double additional_time = 0.0 ) {
		double  whole;
		double  fract = modf( additional_time, &whole );
		struct timeval tmp_time;
		struct timezone tmp_tz;
		gettimeofday( &tmp_time, &tmp_tz );
		double  wsec = tmp_time.tv_sec;
		double  fsec = tmp_time.tv_usec / 1e6;
		BULKIO::PrecisionUTCTime tstamp = BULKIO::PrecisionUTCTime(  );
		tstamp.tcmode = 1;
		tstamp.tcstatus = ( short )1;
		tstamp.toff = 0.0;
		tstamp.twsec = wsec + whole;
		tstamp.tfsec = fsec + fract;
		while( tstamp.tfsec < 0 ) {
			tstamp.twsec -= 1.0;
			tstamp.tfsec += 1.0;
		}
		return tstamp;
	};

	inline  std::string uptime_string( const BULKIO::PrecisionUTCTime & start_time ) {
		BULKIO::PrecisionUTCTime current_time = getSystemTimestamp(  );
		char    timeArray[30];
		double  seconds;
		double  minutes;
		double  hours;
		double  days;
		double  diff_seconds = current_time.twsec - start_time.twsec;
		modf( diff_seconds / 86400.0, &days );
		diff_seconds -= days * 86400.0;
		modf( diff_seconds / 3600.0, &hours );
		diff_seconds -= hours * 3600.0;
		modf( diff_seconds / 60.0, &minutes );
		diff_seconds -= minutes * 60.0;
		seconds = diff_seconds;
		sprintf( timeArray, "%03.0f:%02.0f:%02.0f:%02.0f", days, hours, minutes, seconds );
		std::string time = std::string( timeArray );
		return time;

	}



	inline bool operator<( const BULKIO::PrecisionUTCTime & t1, const BULKIO::PrecisionUTCTime & t2 ) {
		if( t1.twsec != t2.twsec )
			return t1.twsec < t2.twsec;
		if( t1.tfsec != t2.tfsec )
			return t1.tfsec < t2.tfsec;
		return false;
	}
	inline bool operator>( const BULKIO::PrecisionUTCTime & t1, const BULKIO::PrecisionUTCTime & t2 ) {
		if( t1.twsec != t2.twsec )
			return t1.twsec > t2.twsec;
		if( t1.tfsec != t2.tfsec )
			return t1.tfsec > t2.tfsec;
		return false;
	}
	inline  BULKIO::PrecisionUTCTime operator+( const BULKIO::PrecisionUTCTime & t1, const BULKIO::PrecisionUTCTime & t2 ) {
		BULKIO::PrecisionUTCTime tstamp;
		tstamp.tcmode = 1;
		tstamp.tcstatus = ( short )1;
		tstamp.toff = 0.0;
		tstamp.twsec = t1.twsec + t2.twsec;
		tstamp.tfsec = t1.tfsec + t2.tfsec;
		double  whole;
		tstamp.tfsec = modf( tstamp.tfsec, &whole );
		tstamp.twsec += whole;
		return tstamp;
	}
	inline  BULKIO::PrecisionUTCTime operator+( const BULKIO::PrecisionUTCTime & t1, const double &t2 ) {
		BULKIO::PrecisionUTCTime tstamp;
		tstamp.tcmode = 1;
		tstamp.tcstatus = ( short )1;
		tstamp.toff = 0.0;
		double  whole;
		tstamp.tfsec = modf( tstamp.tfsec + t2, &whole );
		tstamp.twsec = t1.twsec + whole;
		return tstamp;
	}

    inline BULKIO::PrecisionUTCTime operator-(const BULKIO::PrecisionUTCTime& lhs, const double& rhs) {
        BULKIO::PrecisionUTCTime(tmp);
        tmp = lhs;
        tmp.tfsec -= rhs;
        if (tmp.tfsec < 0.0) {
            while (tmp.tfsec < 0.0) {            
                tmp.tfsec++;
                tmp.twsec--;
            }
        } else if (tmp.tfsec >= 1.0) {
            while (tmp.tfsec >= 1.0) {
                tmp.tfsec--;
                tmp.twsec++;
            }
        } else {
            //Do Nothing, the change in timestamp did not roll into the next whole number
        }
        return tmp;
    };

    inline BULKIO::PrecisionUTCTime operator-(const BULKIO::PrecisionUTCTime& lhs, const BULKIO::PrecisionUTCTime& rhs) {
        BULKIO::PrecisionUTCTime(tmp);
        tmp = lhs;
        tmp.tfsec -= rhs.tfsec;
        tmp.twsec -= rhs.twsec;
        if (tmp.tfsec < 0.0) {
            while (tmp.tfsec < 0.0) {            
                tmp.tfsec++;
                tmp.twsec--;
            }
        } else if (tmp.tfsec >= 1.0) {
            while (tmp.tfsec >= 1.0) {
                tmp.tfsec--;
                tmp.twsec++;
            }
        } else {
            //Do Nothing, the change in timestamp did not roll into the next whole number
        }
        return tmp;
    };

    template<typename RET_TYPE>
    RET_TYPE caster(const BULKIO::PrecisionUTCTime& val)
    {
        return (RET_TYPE)(val.twsec+val.tfsec);
    };

    enum Round
    {
        ROUND_DOWN,    //0
        ROUND_UP,      //1
        ROUND_NEAREST  //2
    };

    BULKIO::PrecisionUTCTime determineTimeStamp(const BULKIO::PrecisionUTCTime& currentTimestamp, const int& sample, const double& sampleRate);
    int determineSample(const BULKIO::PrecisionUTCTime& beginTimestamp, const BULKIO::PrecisionUTCTime& timestamp, const double& sampleRate, const BIO_HELPER::Round &mode);
};


/*********************************************************************************************/
/**************************            STD_STRING_HELPER            **************************/
/*********************************************************************************************/
namespace STD_STRING_HELPER {
	std::string replace_string( std::string whole_string, const std::string & cur_substr, const std::string & new_substr );
	void    toUpper( std::string & s );
	void    toLower( std::string & s );
	double  SPS_string_to_number( std::string numerical_string );
	double  BPS_string_to_number( std::string numerical_string );
	double  HZ_string_to_number( std::string numerical_string );
	double  generic_string_to_number( std::string numerical_string, std::string units, double k_val );
	std::vector < std::string > string_to_vector( const std::string & csv, const std::string & regex_sep );
	std::set < std::string > string_to_set( const std::string & csv, const std::string & regex_sep );
	std::string set_to_csv( const std::set < std::string > &container );
	std::string vector_to_csv( const std::vector < std::string > &container );
	std::vector < std::string > set_to_vector( const std::set < std::string > &container );
	inline  std::vector < std::string > csv_to_vector( const std::string & csv ) {
		return string_to_vector( csv, "," );
	} inline std::set < std::string > csv_to_set( const std::string & csv ) {
		return string_to_set( csv, "," );
	}

	bool    remove_if_string_ends_with( std::string & whole_string, const std::string & ending_string );
	bool    remove_if_string_starts_with( std::string & whole_string, const std::string & starting_string );
}


/*********************************************************************************************/
/**************************       DOMAIN_MANAGER_HELPERS            **************************/
/*********************************************************************************************/

namespace DOMAIN_MANAGER_HELPERS {

	// Gets a reference to a domain mgr.
	CF::DomainManager_var domainManager_id_to_var( std::string id );
	std::string domainManager_id_to_name( std::string id );

	CF::DomainManager_var find_domainManager( std::string domainMgr_id="");
	CF::FileManager_var find_fileManager( std::string domainMgr_id="");


	// Get a ref to the app factory. If one does not exist, then try to create.
	CF::ApplicationFactory_var getAppFactoryForWaveform( CF::DomainManager_var DomainManager_out, std::string waveform_name, std::string sad_profile = "" );

	//Find Devices
	CF::Device_var getDeviceObjectFromDeviceInstantiationId( CF::DomainManager_var DomainManager_out, std::string instantiation_id );
	std::vector < CF::Device_var > getDeviceObjectsFromDeviceLabel( CF::DomainManager_var DomainManager_out, std::string device_label );
	std::vector < CF::Device_var > getDeviceObjectsFromDeviceName( CF::DomainManager_var DomainManager_out, std::string device_name );
	std::vector < CF::Device_var > getDeviceObjectsThatSatisfiedApplicationUsesDeviceDependency( CF::DomainManager_var DomainManager_out, CF::Application_var appPtr );
	std::vector < CF::Device_var > getDeviceObjectsThatSatisfiedComponentsUsesDeviceDependency( CF::DomainManager_var DomainManager_out, std::string component_id );
	std::vector < CF::Device_var > getDeviceObjectsInDomain( CF::DomainManager_var DomainManager_out, std::vector < CF::DeviceManager_var > devMgrPriority = std::vector < CF::DeviceManager_var > (  ) );
	std::vector < CF::Device_var > getDeviceObjectsInSameNode( CF::DomainManager_var DomainManager_out, std::string device_id );
	std::vector < CF::Device_var > getDevicesObjectsInDeviceManager( CF::DomainManager_var DomainManager_out, CF::DeviceManager_var devMgr );
	std::vector < CF::DeviceManager_var > getDeviceManagersAssociatedWithComponent( CF::DomainManager_var DomainManager_out, std::string component_id );
	CORBA::Object_var getServiceObjectFromServiceName( CF::DomainManager_var DomainManager_out, std::string service_name );


	//Find Waveforms
	CF::Application_var getApplicationObjectFromWaveformID( CF::DomainManager_var DomainManager_out, std::string waveform_id );
	CF::Application_var getApplicationObjectFromComponentInstantiationId( CF::DomainManager_var DomainManager_out, std::string component_id );
	std::vector < CF::Application_var > getApplicationObjectsFromWaveformName( CF::DomainManager_var DomainManager_out, std::string waveform_name );
	//Find Components
	CF::Resource_var getComponentObjectFromComponentInstantiationId( CF::DomainManager_var DomainManager_out, std::string component_id );
	std::vector < CF::Resource_var > getComponentObjectsFromComponentName( CF::DomainManager_var DomainManager_out, std::string component_name );

	CF::Port_var port_from_object( CORBA::Object_ptr corba_obj, std::string port_name );
	CORBA::Object_var port_objectVar_from_object( CORBA::Object_ptr corba_obj, std::string port_name );


	template < class T > typename T::_ptr_type _narrowSafe( const CORBA::Object_ptr obj ) {
		try {
			// check to make sure that it's a valid reference before attempting to narrow
			if( CORBA::Object::_PR_is_valid( obj ) ) {
				return T::_narrow( obj );
			} else {
				return T::_nil(  );
			}
		}
		catch( ... ) {
			return T::_nil(  );
		}
	};
}


#endif
