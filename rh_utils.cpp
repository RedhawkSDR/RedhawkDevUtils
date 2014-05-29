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
#include "rh_utils.h"
#include <math.h>
#include <iostream>

namespace STD_STRING_HELPER {

    std::string replace_string( std::string whole_string, const std::string & cur_substr, const std::string & new_substr ) {
        boost::replace_all(whole_string,cur_substr,new_substr);
        return whole_string;
    }

    void    toUpper( std::string & s ) {
        for( std::string::iterator p = s.begin(  ); p != s.end(  ); ++p ) {
            *p = toupper( *p );
        }
    }

    void    toLower( std::string & s ) {
        for( std::string::iterator p = s.begin(  ); p != s.end(  ); ++p ) {
            *p = tolower( *p );
        }
    }

    double  SPS_string_to_number( std::string numerical_string ) {
        return generic_string_to_number( numerical_string, "SPS", 1e3 );
    }

    double  BPS_string_to_number( std::string numerical_string ) {
        return generic_string_to_number( numerical_string, "BPS", 1024 );
    }
    double  HZ_string_to_number( std::string numerical_string ) {
        return generic_string_to_number( numerical_string, "HZ", 1e3 );
    }
    double  generic_string_to_number( std::string numerical_string, std::string units, double k_val ) {
        toUpper( numerical_string );
        if( numerical_string == "*" )
            return std::numeric_limits < typeof( float )>::max(  );

        double  prefix_multiplier = 1;
        size_t  pos;
        numerical_string = boost::algorithm::trim_copy( numerical_string );
        if( ( pos = numerical_string.find( "K" + units ) ) != std::string::npos ) {
            prefix_multiplier = std::pow( k_val, 1 );
            numerical_string.erase( pos );
        } else if( ( pos = numerical_string.find( "M" + units ) ) != std::string::npos ) {
            prefix_multiplier = std::pow( k_val, 2 );
            numerical_string.erase( pos );
        } else if( ( pos = numerical_string.find( "G" + units ) ) != std::string::npos ) {
            prefix_multiplier = std::pow( k_val, 3 );
            numerical_string.erase( pos );
        } else if( ( pos = numerical_string.find( units ) ) != std::string::npos ) {
            prefix_multiplier = 1;
            numerical_string.erase( pos );
        }
        numerical_string = boost::algorithm::trim_copy( numerical_string );
        return ( prefix_multiplier * std::atof( numerical_string.c_str(  ) ) );

    }


    std::vector < std::string > string_to_vector( const std::string & csv, const std::string & regex_sep ) {
        std::vector < std::string > ret;
        boost::regex re( regex_sep.c_str(  ) ); // Create the reg exp
        boost::sregex_token_iterator    // Create an iterator using a
            p( csv.begin(  ), csv.end(  ), re, -1 );    // sequence and that reg exp
        boost::sregex_token_iterator end;       // Create an end-of-reg-exp
        while( p != end ) {
            std::string tmp = std::string( *p++ );
            if( !tmp.empty(  ) ) {
                ret.push_back( boost::algorithm::trim_copy( tmp ) );
            }
        }
        return ret;

    }

    std::set < std::string > string_to_set( const std::string & csv, const std::string & regex_sep ) {
        std::set < std::string > ret;
        boost::regex re( regex_sep.c_str(  ) ); // Create the reg exp
        boost::sregex_token_iterator    // Create an iterator using a
            p( csv.begin(  ), csv.end(  ), re, -1 );    // sequence and that reg exp
        boost::sregex_token_iterator end;       // Create an end-of-reg-exp
        while( p != end ) {
            std::string tmp = std::string( *p++ );
            if( !tmp.empty(  ) ) {
                ret.insert( boost::algorithm::trim_copy( tmp ) );
            }
        }
        return ret;

    }

    std::vector < std::string > set_to_vector( const std::set < std::string > &container ) {
        std::vector < std::string > ret;
        for( std::set < std::string >::const_iterator it = container.begin(  ); it != container.end(  ); it++ )
            ret.push_back( *it );
        return ret;
    }

    std::string set_to_csv( const std::set < std::string > &container ) {
        std::string str = "";
        for( std::set < std::string >::const_iterator it = container.begin(  ); it != container.end(  ); it++ ) {
            str += *it + ",";
        }
        if( !str.empty(  ) )
            str.erase( str.size(  ) - 1 );
        return str;
    }
    std::string vector_to_csv( const std::vector < std::string > &container ) {
        std::string str = "";
        for( std::vector < std::string >::const_iterator it = container.begin(  ); it != container.end(  ); it++ ) {
            str += *it + ",";
        }
        if( !str.empty(  ) )
            str.erase( str.size(  ) - 1 );
        return str;
    }


    bool    remove_if_string_ends_with( std::string & whole_string, const std::string & ending_string ) {
        try {
            if( 0 == whole_string.compare( whole_string.size(  ) - ending_string.size(  ), ending_string.size(  ), ending_string ) ) {
                whole_string = whole_string.substr( 0, whole_string.size(  ) - ending_string.size(  ) );
                return true;
            }
        }
        catch( ... ) {
        };

        return false;
    }

    bool    remove_if_string_starts_with( std::string & whole_string, const std::string & starting_string ) {
        try {
            if( 0 == whole_string.compare( 0, starting_string.size(  ), starting_string ) ) {
                whole_string = whole_string.substr( starting_string.size(  ), whole_string.size(  ) - starting_string.size(  ) );
                return true;
            }
        }
        catch( ... ) {
        };
        return false;
    }

}

namespace DOMAIN_MANAGER_HELPERS {

	CF::DomainManager_var find_domainManager(std::string domainMgr_id) {
		return domainManager_id_to_var(domainMgr_id);
	}
	CF::FileManager_var find_fileManager( std::string domainMgr_id){
		CF::DomainManager_var dm = find_domainManager(domainMgr_id);
		if(!CORBA::is_nil(dm))
			return dm->fileMgr();
		return CF::FileManager::_nil();
	}

    CF::DomainManager_var domainManager_id_to_var( std::string id ) {
        CF::DomainManager_var domainMgr_var = CF::DomainManager::_nil(  );
        CosNaming::BindingIterator_var it;
        CosNaming::BindingList_var bl;
        CosNaming::NamingContext_var context = DOMAIN_MANAGER_HELPERS::_narrowSafe < CosNaming::NamingContext > ( ossie::corba::InitialNamingContext(  ) );
        context->list( 100, bl, it );
        for( unsigned int ii = 0; ii < bl->length(  ); ++ii ) {
            try {
                std::string domString = std::string( bl[ii].binding_name[0].id ) + "/" + std::string( bl[ii].binding_name[0].id );
                CosNaming::Name_var cosName = omni::omniURI::stringToName( domString.c_str(  ) );
                CORBA::Object_var bobj = context->resolve( cosName );
                domainMgr_var = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::DomainManager > ( bobj );
                std::string dm_id = ossie::corba::returnString( domainMgr_var->identifier(  ) );
                if( id.empty(  ) || id == dm_id ) {
                    return domainMgr_var;
                }
            }
            catch   ( ... ) {
            };
        }
        return CF::DomainManager::_nil(  );
    }

    std::string domainManager_id_to_name( std::string id ) {
        CF::DomainManager_var domainMgr_var = CF::DomainManager::_nil(  );
        CosNaming::BindingIterator_var it;
        CosNaming::BindingList_var bl;
        CosNaming::NamingContext_var context = DOMAIN_MANAGER_HELPERS::_narrowSafe < CosNaming::NamingContext > ( ossie::corba::InitialNamingContext(  ) );
        context->list( 100, bl, it );
        for( unsigned int ii = 0; ii < bl->length(  ); ++ii ) {
            try {
                std::string domString = std::string( bl[ii].binding_name[0].id )
                    + "/" + std::string( bl[ii].binding_name[0].id );
                CosNaming::Name_var cosName = omni::omniURI::stringToName( domString.c_str(  ) );
                CORBA::Object_var bobj = context->resolve( cosName );
                domainMgr_var = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::DomainManager > ( bobj );
                std::string dm_id = ossie::corba::returnString( domainMgr_var->identifier(  ) );
                if( id.empty(  ) || id == dm_id ) {
                    return std::string( bl[ii].binding_name[0].id );
                }
            }
            catch( ... ) {
            };
        }
        return "";
    }



    CF::ApplicationFactory_var getAppFactoryForWaveform( CF::DomainManager_var DomainManager_out, std::string waveform_name, std::string sad_profile ) {
        // Check to see if application factory is already installed
        CF::ApplicationFactory_var myAppFact = CF::ApplicationFactory::_nil(  );
        try {

            CF::DomainManager::ApplicationFactorySequence_var appSeq = DomainManager_out->applicationFactories(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                bool    found_by_sad = ( sad_profile == ossie::corba::returnString( appSeq[i]->softwareProfile(  ) ) );
                bool    found_by_name = ( waveform_name == ossie::corba::returnString( appSeq[i]->name(  ) ) );
                if( found_by_sad || found_by_name ) {
                    myAppFact = appSeq[i];
                    break;
                }
            }
            // If no application factory installed, then try to install
            if( myAppFact->_NP_is_nil(  ) ) {
                if( sad_profile.empty(  ) ){
                    if(waveform_name.find_first_of('/') == 0 || waveform_name.find_first_of("./") == 0)
                    	sad_profile = waveform_name;
                    else
                    	sad_profile = "/waveforms/" + waveform_name + "/" + waveform_name + ".sad.xml";
                }
                DomainManager_out->installApplication( sad_profile.c_str(  ) );
                CF::DomainManager::ApplicationFactorySequence_var appSeq = DomainManager_out->applicationFactories(  );
                for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                    bool    found_by_sad = ( sad_profile == ossie::corba::returnString( appSeq[i]->softwareProfile(  ) ) );
                    bool    found_by_name = ( waveform_name == ossie::corba::returnString( appSeq[i]->name(  ) ) );
                    if( found_by_sad || found_by_name ) {
                        myAppFact = appSeq[i];
                        break;
                    }
                }
            }
        }
        catch( ... ) {

        }

        return myAppFact;
    }

    CF::Device_var getDeviceObjectFromDeviceInstantiationId( CF::DomainManager_var DomainManager_out, std::string instantiation_id ) {
        CF::Device_var myDevObj = CF::Device::_nil(  );
        try {
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                CF::DeviceSequence_var devSeq = devMgrSeq[devMgrSeq_iter]->registeredDevices(  );
                for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                    if( ossie::corba::returnString( devSeq[devSeq_iter]->identifier(  ) ) == instantiation_id ) {
                        myDevObj = devSeq[devSeq_iter];
                        return myDevObj;
                    }
                }
            }
        }
        catch( ... ) {
        }

        return myDevObj;
    }

    CORBA::Object_var getServiceObjectFromServiceName( CF::DomainManager_var DomainManager_out, std::string service_name ) {
        CORBA::Object_var myServiceObj = CORBA::Object::_nil(  );
        try {
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                CF::DeviceManager::ServiceSequence_var registeredServices = devMgrSeq[devMgrSeq_iter]->registeredServices(  );
                for( CORBA::ULong servSeq_pos = 0; servSeq_pos < registeredServices->length(  ); servSeq_pos++ ) {
                    if( ossie::corba::returnString( registeredServices[servSeq_pos].serviceName ) == service_name ) {
                        myServiceObj = registeredServices[servSeq_pos].serviceObject;
                        return myServiceObj;
                    }
                }
            }
        }
        catch( ... ) {
        }

        return myServiceObj;
    }


    std::vector < CF::Device_var > getDevicesObjectsInDeviceManager( CF::DomainManager_var DomainManager_out, CF::DeviceManager_var devMgr ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            CF::DeviceSequence_var devSeq = devMgr->registeredDevices(  );
            for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                myDevObj_vect.push_back( devSeq[devSeq_iter] );
            }
        }
        catch( ... ) {
        }
        return myDevObj_vect;
    }

    std::vector < CF::Device_var > getDeviceObjectsInDomain( CF::DomainManager_var DomainManager_out, std::vector < CF::DeviceManager_var > devMgrPriority ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            std::vector < CF::DeviceManager_var > all_dev_mgrs = devMgrPriority;
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                bool    found = false;
                for( std::vector < CF::DeviceManager_var >::iterator it = devMgrPriority.begin(  ); it != devMgrPriority.end(  ); it++ ) {
                    if( ossie::corba::returnString( ( *it )->identifier(  ) )
                        == ossie::corba::returnString( devMgrSeq[devMgrSeq_iter]->identifier(  ) ) ) {
                        found = true;
                        break;
                    }
                }
                if( !found )
                    all_dev_mgrs.push_back( devMgrSeq[devMgrSeq_iter] );
            }

            for( std::vector < CF::DeviceManager_var >::iterator it = all_dev_mgrs.begin(  ); it != all_dev_mgrs.end(  ); it++ ) {
                CF::DeviceSequence_var devSeq = ( *it )->registeredDevices(  );
                for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                    myDevObj_vect.push_back( devSeq[devSeq_iter] );
                }
            }

        }
        catch( ... ) {
        }
        return myDevObj_vect;

    }

    std::vector < CF::Device_var > getDeviceObjectsFromDeviceLabel( CF::DomainManager_var DomainManager_out, std::string device_label ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                CF::DeviceSequence_var devSeq = devMgrSeq[devMgrSeq_iter]->registeredDevices(  );
                for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                    if( ossie::corba::returnString( devSeq[devSeq_iter]->label(  ) ) == device_label ) {
                        myDevObj_vect.push_back( devSeq[devSeq_iter] );
                    }
                }
            }
        }
        catch( ... ) {
        }
        return myDevObj_vect;
    }

    std::vector < CF::Device_var > getDeviceObjectsFromDeviceName( CF::DomainManager_var DomainManager_out, std::string device_name ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                CF::DeviceSequence_var devSeq = devMgrSeq[devMgrSeq_iter]->registeredDevices(  );
                for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                    if( ossie::corba::returnString( devSeq[devSeq_iter]->softwareProfile(  ) ).find( "/" + device_name + "/" ) != std::string::npos ) {
                        myDevObj_vect.push_back( devSeq[devSeq_iter] );
                    }
                }
            }
        }
        catch( ... ) {
        }
        return myDevObj_vect;
    }

    std::vector < CF::Device_var > getDeviceObjectsInSameNode( CF::DomainManager_var DomainManager_out, std::string device_id ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                CF::DeviceSequence_var devSeq = devMgrSeq[devMgrSeq_iter]->registeredDevices(  );
                bool    foundInNode = false;
                myDevObj_vect.clear(  );
                for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                    myDevObj_vect.push_back( devSeq[devSeq_iter] );
                    if( ossie::corba::returnString( devSeq[devSeq_iter]->identifier(  ) ) == device_id )
                        foundInNode = true;
                }
                if( foundInNode )
                    return myDevObj_vect;
            }
        }
        catch( ... ) {
        }
        myDevObj_vect.clear(  );
        return myDevObj_vect;

    }

    std::vector < CF::Device_var > getDeviceObjectsThatSatisfiedApplicationUsesDeviceDependency( CF::DomainManager_var DomainManager_out, CF::Application_var appPtr ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            CF::DeviceAssignmentSequence_var devSeq = appPtr->componentDevices(  );
            for( unsigned int i = 0; i < devSeq->length(  ); i++ ) {
                std::string device_id( devSeq[i].assignedDeviceId );
                myDevObj_vect.push_back( getDeviceObjectFromDeviceInstantiationId( DomainManager_out, device_id ) );
            }
        } catch( ... ) {
        }
        return myDevObj_vect;
    }

    std::vector < CF::DeviceManager_var > getDeviceManagersAssociatedWithComponent( CF::DomainManager_var DomainManager_out, std::string component_id ) {

        std::set < CF::DeviceManager_var > myDevManObj_set;
        std::vector < CF::DeviceManager_var > myDevManObj_vect;
        try {
            CF::DomainManager::DeviceManagerSequence_var devMgrSeq = DomainManager_out->deviceManagers(  );
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            // get a list of running applications
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                CF::Components_var compSeq = appSeq[i]->registeredComponents(  );
                // get a reference to running component
                for( CORBA::ULong j = 0; j < compSeq->length(  ); j++ ) {
                    if( std::string( compSeq[j].identifier ) == component_id ) {
                        CF::DeviceAssignmentSequence_var devSeq = appSeq[i]->componentDevices(  );
                        // get a reference to an associated device
                        for( unsigned int i = 0; i < devSeq->length(  ); i++ ) {
                            if( std::string( devSeq[i].componentId ) == component_id ) {
                                std::string device_id( devSeq[i].assignedDeviceId );
                                // determine which device manager that device is running in
                                for( CORBA::ULong devMgrSeq_iter = 0; devMgrSeq_iter < devMgrSeq->length(  ); devMgrSeq_iter++ ) {
                                    CF::DeviceSequence_var devSeq = devMgrSeq[devMgrSeq_iter]->registeredDevices(  );
                                    for( CORBA::ULong devSeq_iter = 0; devSeq_iter < devSeq->length(  ); devSeq_iter++ ) {
                                        if( ossie::corba::returnString( devSeq[devSeq_iter]->identifier(  ) )
                                            == device_id )
                                            myDevManObj_set.insert( devMgrSeq[devMgrSeq_iter] );
                                    }

                                }
                            }
                        }
                        break;
                    }
                }
            }
            for( std::set < CF::DeviceManager_var >::iterator it = myDevManObj_set.begin(  ); it != myDevManObj_set.end(  ); it++ )
                myDevManObj_vect.push_back( *it );

        }
        catch( ... ) {
        }
        return myDevManObj_vect;

    }

    std::vector < CF::Device_var > getDeviceObjectsThatSatisfiedComponentsUsesDeviceDependency( CF::DomainManager_var DomainManager_out, std::string component_id ) {
        std::vector < CF::Device_var > myDevObj_vect;
        try {
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                CF::Components_var compSeq = appSeq[i]->registeredComponents(  );
                for( CORBA::ULong j = 0; j < compSeq->length(  ); j++ ) {
                    if( std::string( compSeq[j].identifier ) == component_id ) {
                        CF::DeviceAssignmentSequence_var devSeq = appSeq[i]->componentDevices(  );
                        for( unsigned int i = 0; i < devSeq->length(  ); i++ ) {
                            if( std::string( devSeq[i].componentId ) == component_id ) {
                                std::string device_id( devSeq[i].assignedDeviceId );
                                myDevObj_vect.push_back( getDeviceObjectFromDeviceInstantiationId( DomainManager_out, device_id ) );
                            }
                        }
                        return myDevObj_vect;

                    }
                }
            }
        }
        catch( ... ) {
        }
        return myDevObj_vect;

    }

    CF::Application_var getApplicationObjectFromWaveformID( CF::DomainManager_var DomainManager_out, std::string waveform_id ) {
        CF::Application_var myApp = CF::Application::_nil(  );
        try {
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                if( ossie::corba::returnString( appSeq[i]->name(  ) )
                    == waveform_id ) {
                    myApp = appSeq[i];
                    return myApp;
                }
            }
        }
        catch( ... ) {
        }
        return myApp;
    }

    std::vector < CF::Application_var > getApplicationObjectsFromWaveformName( CF::DomainManager_var DomainManager_out, std::string waveform_name ) {
        std::vector < CF::Application_var > myApps;
        try {
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                if( ossie::corba::returnString( appSeq[i]->profile(  ) ).find( "/" + waveform_name + "/" ) != std::string::npos ) {
                    myApps.push_back( appSeq[i] );
                }
            }
        }
        catch( ... ) {
        }
        return myApps;
    }

    CF::Application_var getApplicationObjectFromComponentInstantiationId( CF::DomainManager_var DomainManager_out, std::string component_id ) {
        CF::Application_var myApp = CF::Application::_nil(  );
        try {
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                CF::Components_var compSeq = appSeq[i]->registeredComponents(  );
                for( CORBA::ULong j = 0; j < compSeq->length(  ); j++ ) {
                    if( std::string( compSeq[j].identifier ) == component_id ) {
                        myApp = appSeq[i];
                        return myApp;
                    }
                }
            }
        }
        catch( ... ) {
        }
        return myApp;
    }

    CF::Resource_var getComponentObjectFromComponentInstantiationId( CF::DomainManager_var DomainManager_out, std::string component_id ) {
        CF::Resource_var myComp;
        try {
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                CF::Components_var compSeq = appSeq[i]->registeredComponents(  );
                for( CORBA::ULong j = 0; j < compSeq->length(  ); j++ ) {
                    if( std::string( compSeq[j].identifier ) == component_id ) {
                        myComp = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Resource > ( compSeq[j].componentObject );
                        return myComp;
                    }
                }
            }
        }
        catch( ... ) {
        }
        return myComp;
    }

    std::vector < CF::Resource_var > getComponentObjectsFromComponentName( CF::DomainManager_var DomainManager_out, std::string component_name ) {
        std::vector < CF::Resource_var > myComps;
        try {
            CF::DomainManager::ApplicationSequence_var appSeq = DomainManager_out->applications(  );
            for( CORBA::ULong i = 0; i < appSeq->length(  ); i++ ) {
                CF::Components_var compSeq = appSeq[i]->registeredComponents(  );
                for( CORBA::ULong j = 0; j < compSeq->length(  ); j++ ) {
                    if( std::string( compSeq[j].softwareProfile ).find( "/" + component_name + "/" ) != std::string::npos ) {
                        CF::Resource_var res = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Resource > ( compSeq[j].componentObject );
                        if( !CORBA::is_nil( res ) )
                            myComps.push_back( res );
                    }
                }
            }
        }
        catch( ... ) {
        }
        return myComps;
    }

    CF::Port_var port_from_object( CORBA::Object_ptr corba_obj, std::string port_name ) {
        CF::Port_var port = CF::Port::_nil(  );

        try {
            CF::Resource_var connecting_resource = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Resource > ( corba_obj );
            port = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Port > ( connecting_resource->getPort( port_name.c_str(  ) ) );
            return port;
        }
        catch( ... ) {
        };

        try {
            CF::Application_var connecting_app = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Application > ( corba_obj );
            port = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Port > ( connecting_app->getPort( port_name.c_str(  ) ) );
            return port;
        }
        catch( ... ) {
        };

        return CF::Port::_nil(  );
    }
    ;

    CORBA::Object_var port_objectVar_from_object( CORBA::Object_ptr corba_obj, std::string port_name ) {
        CORBA::Object_var port = CORBA::Object::_nil(  );

        try {
            CF::Resource_var connecting_resource = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Resource > ( corba_obj );
            port = connecting_resource->getPort( port_name.c_str(  ) );
            return port;
        }
        catch( ... ) {
        };

        try {
            CF::Application_var connecting_app = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::Application > ( corba_obj );
            port = connecting_app->getPort( port_name.c_str(  ) );
            return port;
        }
        catch( ... ) {
        };

        try {
            CF::PortSupplier_var connecting_ps = DOMAIN_MANAGER_HELPERS::_narrowSafe < CF::PortSupplier > ( corba_obj );
            port = connecting_ps->getPort( port_name.c_str(  ) );
            return port;
        }
        catch( ... ) {
        };

        return CORBA::Object::_nil(  );
    }
    ;

}

// This method takes a beginning timestamp, a sample rate, and a sample,
// and returns the timestamp associated with the relative sample
BULKIO::PrecisionUTCTime BIO_HELPER::determineTimeStamp(
        const BULKIO::PrecisionUTCTime& currentTimestamp, 
        const int& sample, 
        const double& sampleRate)
{
    BULKIO::PrecisionUTCTime newTimestamp = currentTimestamp;
    double timeDiff = (double)sample/sampleRate; // can be negative, positive, or zero
    newTimestamp.tfsec += timeDiff;
    if (newTimestamp.tfsec < 0.0) {
        while (newTimestamp.tfsec < 0.0) {
            newTimestamp.tfsec = newTimestamp.tfsec + 1.0;
            newTimestamp.twsec = newTimestamp.twsec - 1.0;
        }
    } else if(newTimestamp.tfsec >= 1.0) {
        while (newTimestamp.tfsec >= 1.0) {
            newTimestamp.tfsec--;
            newTimestamp.twsec++;
        }
    } else {
        // Do nothing.. the change in the timestamp did not roll into a new whole number
    }
    return newTimestamp;
};

// This method takes a beginning timestamp corresponding to the timestamp associated with a packet,
// along with a sampleRate, a timestamp pertaining to a specific sample, and a enumerated value
// pertaining to rounding preference, and returns a integer specific to the sample in relation
// to the current packet 
int BIO_HELPER::determineSample(
        const BULKIO::PrecisionUTCTime& beginTimestamp, 
        const BULKIO::PrecisionUTCTime& timestamp, 
        const double &sampleRate, 
        const BIO_HELPER::Round &mode)
{
    double diffTime = (timestamp.twsec - beginTimestamp.twsec) + (timestamp.tfsec - beginTimestamp.tfsec);
    double sample = diffTime*sampleRate;
    int ret_val = 0;
    switch (mode)
    {
        case ROUND_DOWN:
            ret_val = (int)floor(sample);
            break;
        case ROUND_UP:
            ret_val = (int)ceil(sample);
            break;
        case ROUND_NEAREST:
            ret_val = (int)round(sample);
            break;
        default:
            std::cout << "ERROR: in the determineSample Helper method" << std::endl;
    }
    return ret_val;
};
