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
#ifndef ABSTRACTED_FILE_IO_H
#define ABSTRACTED_FILE_IO_H

#include <vector>
#include <queue>
#include <complex>
#include <set>
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
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <ossie/prop_helpers.h>
#include <ossie/Resource_impl.h>
#include "vectorMagic.h"

#define BUFF_MAX 2048000



#define _DEBUG_

namespace ABSTRACTED_FILE_IO {

    const std::string sca_uri_prefix = "sca://";
    const std::string local_uri_prefix = "file://";


    // File systesm  with abstracted IO

    enum FILESYSTEM_TYPE {
        LOCAL_FILESYSTEM = 0,
        SCA_FILESYSTEM = 1
    };

    
    struct local_file_struct {

        local_file_struct() {
            curFile = NULL;
        };
        
        std::fstream* curFile;
        std::vector<char> buffer;
    };

    struct sca_file_struct {

        sca_file_struct() {
            curFile = CF::File::_nil();
        }
        CF::File_var curFile;
        CF::OctetSequence_var buffer;
    };

    struct file_listing {
        std::string filename_full;
        std::string filename_basename;
        unsigned long long file_size;
        FILESYSTEM_TYPE file_system_type;
    };

    class abstracted_file_io {
    public:
        abstracted_file_io(CF::FileManager_var _sca_file_manager = CF::FileManager::_nil());
        ~abstracted_file_io();
        inline bool is_sca_file_manager_valid(){
        	return ! CORBA::is_nil(sca_file_manager);
        }

        inline void update_sca_file_manager(CF::FileManager_var _sca_file_manager) {
            sca_file_manager = _sca_file_manager;
        }

        FILESYSTEM_TYPE source_uri_to_filesystem(const std::string &_source_uri);
        
        std::vector<file_listing> create_listing_from_source_uri(const std::string &_source_uri);
        int file_count(const std::string &_source_file_uri);

        bool exists(const std::string &_source_file_uri);

        bool delete_file(const std::string &_source_file_uri);
        bool open_file(const std::string &_source_file_uri, bool rw = false, bool write_ate = true);
        bool read(const std::string &_source_file_uri, std::vector<char> * dataBuffer, size_t size_bytes);
        bool make_dir(const std::string &_source_dir_uri);
        bool file_seek(const std::string &_source_file_uri, unsigned long long offset_from_beginning);
        unsigned long long file_size(const std::string &_source_file_uri);
        unsigned long long file_tell(const std::string &_source_file_uri);
        bool close_file(const std::string &_source_file_uri);
        bool move_file(const std::string &_source_file_uri, const std::string &_dest_file_uri);
        bool copy_file(const std::string &_source_file_uri, const std::string &_dest_file_uri);

        bool write(const std::string &_source_file_uri, char * dataBuffer, size_t dataLength, bool forceFlush = false);

        inline bool write(const std::string &_source_file_uri, std::string * dataBuffer, bool forceFlush = false) {
            return write(_source_file_uri, (char*) &(*dataBuffer)[0], dataBuffer->size(), forceFlush);
        }

        template <typename IN_DATA_TYPE, typename IN_ALLOCATOR>
        bool write(const std::string &_source_file_uri, std::vector<IN_DATA_TYPE, IN_ALLOCATOR> * dataBuffer, bool forceFlush = false) {
            return write(_source_file_uri, (char*) &(*dataBuffer)[0], dataBuffer->size(), forceFlush);
        }
        
        std::string uri_to_file(const std::string &_source_file_uri);
        std::string normalize_uri_path(const std::string& uri_full);
        bool uri_path_extraction(const std::string& uri_full, std::string& dirname, std::string& basename, FILESYSTEM_TYPE& file_sys_type, bool force_as_dir = false);

        bool file_to_sca_file(const std::string& _source_file_uri, const std::string& _sca_file_uri );

    private:
        int sca_file_count(const std::string &_source_file_uri);
        int local_file_count(const std::string &_source_file_uri);

        bool sca_exists(const std::string &_source_file_uri);
        bool local_exists(const std::string &_source_file_uri);

        std::vector<file_listing> create_listing_from_sca_source_uri(const std::string &_source_uri);
        std::vector<file_listing> create_listing_from_local_source_uri(const std::string &_source_uri);

        std::map<std::string, local_file_struct > local_file_mapping;
        std::map<std::string, sca_file_struct > sca_file_mapping;
        CF::FileManager_var sca_file_manager;

        inline std::string replace_string(std::string whole_string, const std::string& cur_substr, const std::string& new_substr) {
            try {
                size_t substring_pos = whole_string.find(cur_substr);
                whole_string.erase(substring_pos, cur_substr.size());
                whole_string.insert(substring_pos, new_substr);
            } catch (...) {
            }
            return whole_string;
        }



    };


}





#endif
