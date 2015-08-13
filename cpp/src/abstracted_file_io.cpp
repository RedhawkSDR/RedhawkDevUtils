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
/**************************************************************************

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

 **************************************************************************/

#include "abstracted_file_io.h"
#include "boost_compat.h"

using namespace ABSTRACTED_FILE_IO;


// CONSTRUCTORS / DESTRUCTOR

abstracted_file_io::abstracted_file_io(CF::FileManager_var _sca_file_manager) {
    sca_file_manager = _sca_file_manager;

}

abstracted_file_io::~abstracted_file_io() {
    // clear local mapping 
    for (std::map<std::string, local_file_struct >::iterator iter = local_file_mapping.begin(); iter != local_file_mapping.end(); iter++) {

        if (iter->second.curFile->is_open()) {
            iter->second.curFile->close();
        }
        delete iter->second.curFile;

    }
    local_file_mapping.clear();
    // clear sca mapping 
    for (std::map<std::string, sca_file_struct >::iterator iter = sca_file_mapping.begin(); iter != sca_file_mapping.end(); iter++) {
        if (!CORBA::is_nil(iter->second.curFile)) {
            iter->second.curFile->close();
            iter->second.curFile = CF::File::_nil();
        }
    }
    sca_file_mapping.clear();
}


// Helpers

FILESYSTEM_TYPE abstracted_file_io::source_uri_to_filesystem(const std::string &_source_uri) {
    std::string norm = normalize_uri_path(_source_uri);
    std::string dir, base;
    FILESYSTEM_TYPE type = LOCAL_FILESYSTEM;
    uri_path_extraction(norm, dir, base, type);
    return type;
}

std::string abstracted_file_io::uri_to_file(const std::string &_source_file_uri) {
    std::string norm = normalize_uri_path(_source_file_uri);
    std::string source_location, dir, base;
    FILESYSTEM_TYPE type = LOCAL_FILESYSTEM;
    uri_path_extraction(norm, dir, base, type);
    source_location = dir + base;
    size_t loc = source_location.find("/.");
    if(loc != std::string::npos && loc == source_location.size()-2)
        source_location.erase(source_location.end()-1);
        
    return source_location;

}

/////////////////////////////////

int abstracted_file_io::file_count(const std::string &_source_file_uri) {
    std::string norm = normalize_uri_path(_source_file_uri);
    FILESYSTEM_TYPE type = source_uri_to_filesystem(norm);
    if (type == SCA_FILESYSTEM)
        return sca_file_count(norm);
    return local_file_count(norm);
}

int abstracted_file_io::sca_file_count(const std::string &_source_file_uri) {
    int file_count = 0;
	try{
		std::string source_location = uri_to_file(_source_file_uri) + "/";
		CF::FileSystem::FileInformationSequence_var top_level_results;
		try{
			top_level_results = sca_file_manager->list(source_location.c_str());
		}catch(...){
			return file_count;
		};
		for (CORBA::ULong tlres = 0; tlres < top_level_results->length(); ++tlres){
			if (top_level_results[tlres].kind == CF::FileSystem::PLAIN){
				++file_count;
			}
		}
		return file_count;
	}catch( const std::exception & ex ){
		std::cerr << "SCA FILE COUNT: " << _source_file_uri << " " << ex.what() << " ERROR!\n";
		return file_count;
	}
}

int abstracted_file_io::local_file_count(const std::string &_source_file_uri) {
	std::string source_location = uri_to_file(_source_file_uri);
    boost::filesystem::path fullPath = boost::filesystem::system_complete(BOOST_FILESYSTEM_PATH(source_location));
    if (!boost::filesystem::exists(fullPath)) {
		std::cerr << "LOCAL FILE COUNT: " << fullPath << " Does Not Exist!\n";
        return 0;
    }
    boost::filesystem::directory_iterator end_itr;
    int file_count = 0;
    for( boost::filesystem::directory_iterator itr(fullPath); itr != end_itr; ++itr){
    	try{
    		if(boost::filesystem::is_regular_file(itr->status())){
    			++file_count;
    		}
    	} catch ( const std::exception & ex ){
    		std::cerr << "LOCAL FILE COUNT: " << itr->path().filename() << " " << ex.what() << std::endl;
    		return file_count;
    	}
    }
    return file_count;
}

/////////////////////////////////

bool abstracted_file_io::exists(const std::string &_source_file_uri) {
    std::string norm = normalize_uri_path(_source_file_uri);
    FILESYSTEM_TYPE type = source_uri_to_filesystem(norm);
    if (type == SCA_FILESYSTEM)
        return sca_exists(norm);
    return local_exists(norm);
}

bool abstracted_file_io::sca_exists(const std::string &_source_file_uri) {
        try{
                std::string source_location = uri_to_file(_source_file_uri);
                return sca_file_manager->exists(source_location.c_str());
        }catch(...){
                return false;
        }
}

bool abstracted_file_io::local_exists(const std::string &_source_file_uri) {
    std::string source_location = uri_to_file(_source_file_uri);

    boost::filesystem::path fullPath = boost::filesystem::system_complete(BOOST_FILESYSTEM_PATH(source_location));
    if (!boost::filesystem::exists(fullPath)) {
        return false;
    }
    return true;
}




// CREATE LISTINGS CHECK

std::vector<file_listing> abstracted_file_io::create_listing_from_source_uri(const std::string &_source_uri) {
    if (source_uri_to_filesystem(_source_uri) == SCA_FILESYSTEM)
        return create_listing_from_sca_source_uri(_source_uri);
    return create_listing_from_local_source_uri(_source_uri);
}

std::vector<file_listing> abstracted_file_io::create_listing_from_sca_source_uri(const std::string &_source_uri) {
    std::vector<file_listing> new_listing;
    std::string source_location = uri_to_file(_source_uri);


    CF::FileSystem::FileInformationSequence_var top_level_results;
    try{
    	top_level_results= sca_file_manager->list(source_location.c_str());
     }catch(...){
    	 std::cerr << "SCA FILE LOCATION: " << source_location << " DOES NOT EXIST!\n";
    	 return new_listing;
    };
    for (CORBA::ULong res = 0; res < top_level_results->length(); ++res) {
        if (top_level_results[res].kind == CF::FileSystem::DIRECTORY) {
        	std::string dirName = source_location + "/";
        	CF::FileSystem::FileInformationSequence_var next_level_results;
        	try{
        		next_level_results = sca_file_manager->list(dirName.c_str());
        	}catch(...){
        		continue;
        	};
        	for (CORBA::ULong nres = 0; nres < next_level_results->length(); ++nres) {
                if (next_level_results[nres].kind == CF::FileSystem::PLAIN) {
                	file_listing newFile;
                    newFile.file_system_type = SCA_FILESYSTEM;
                    newFile.filename_full = sca_uri_prefix + dirName + std::string(next_level_results[nres].name);
                    newFile.file_size = next_level_results[nres].size;
                    newFile.filename_basename = next_level_results[nres].name;
                    new_listing.push_back(newFile);
                }
            }

        } else if (top_level_results[res].kind == CF::FileSystem::PLAIN) {
        	file_listing newFile;
            newFile.file_system_type = SCA_FILESYSTEM;
            newFile.file_size = top_level_results[res].size;
            newFile.filename_basename = top_level_results[res].name;
            newFile.filename_full = sca_uri_prefix + source_location;
            if(_source_uri.rfind('/') == _source_uri.size()-1)
            	newFile.filename_full += newFile.filename_basename;
            new_listing.push_back(newFile);
        }

    }
    return new_listing;

}

std::vector<file_listing> abstracted_file_io::create_listing_from_local_source_uri(const std::string &_source_uri) {
    std::vector<file_listing> new_listing;
    std::string source_location = uri_to_file(_source_uri);

    boost::filesystem::path fullPath = boost::filesystem::system_complete(BOOST_FILESYSTEM_PATH(source_location));
    if (!boost::filesystem::exists(fullPath)) {
        std::cerr << "LOCAL FILE LOCATION: " << source_location << " DOES NOT EXIST!\n";
        return new_listing;
    }
    boost::filesystem::directory_iterator end;
    if (boost::filesystem::is_directory(fullPath)) {
        for (boost::filesystem::directory_iterator it(fullPath); it != end; ++it) { // Iterate through each
            if (boost::filesystem::is_directory(*it))
                continue;
            try{
				file_listing newFile;
				newFile.file_system_type = LOCAL_FILESYSTEM;
				newFile.filename_full = local_uri_prefix + it->path().string();
				newFile.file_size = boost::filesystem::file_size(*it);
				newFile.filename_basename = BOOST_PATH_STRING(it->path().filename());
				new_listing.push_back(newFile);
            }catch(...){
                std::cerr << "DOES NOT HAVE PERMISSIONS TO ACCESS " << local_uri_prefix + it->path().string() << "!\n";
            };
        }
    } else {
    	 try{
			file_listing newFile;
			newFile.file_system_type = LOCAL_FILESYSTEM;
			newFile.filename_full = local_uri_prefix + fullPath.string();
			newFile.file_size = boost::filesystem::file_size(fullPath);
			newFile.filename_basename = BOOST_PATH_STRING(fullPath.filename());
			new_listing.push_back(newFile);
    	 }catch(...){
    	          std::cerr << "DOES NOT HAVE PERMISSIONS TO ACCESS " << local_uri_prefix +fullPath.string() << "!\n";
    	 };
    }
    return new_listing;

}

bool abstracted_file_io::open_file(const std::string &_source_file_uri, bool rw, bool write_ate) {
    try {
        std::string source_location = uri_to_file(_source_file_uri);
        sca_file_struct sca_file;
        local_file_struct local_file;
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                if (sca_file_mapping.count(_source_file_uri) >= 1)
                    return true;
                try {
                    sca_file.curFile = sca_file_manager->open(source_location.c_str(), !rw);
                }catch (...) {
                    if (rw)
                        sca_file.curFile = sca_file_manager->create(source_location.c_str());
                }
                
                if (CORBA::is_nil(sca_file.curFile)) {
                    return false;
                }
                if(rw && write_ate)
                    sca_file.curFile->setFilePointer(sca_file.curFile->sizeOf());
                
                sca_file_mapping.insert(std::make_pair(_source_file_uri, sca_file));
                return true;
                break;
            case LOCAL_FILESYSTEM:
                std::ios_base::openmode mode = std::ios::in | std::ios::out;
                if (!rw)
                    mode = std::ios::in;
                
                if (local_file_mapping.count(_source_file_uri) >= 1)
                    return true;
                local_file.curFile = new std::fstream();
                local_file.curFile->open(source_location.c_str(), mode);
                if (!local_file.curFile->is_open() && rw) {
                    mode |= std::ios::trunc;
                    local_file.curFile->open(source_location.c_str(), mode);
                }
                if (!local_file.curFile->is_open()) {
                    delete local_file.curFile;
                    local_file.curFile = NULL;
                    return false;

                }
                if(rw && write_ate){
                    local_file.curFile->seekp(0, std::ios::end);
                    local_file.curFile->seekg(0, std::ios::end);
                }
                local_file_mapping.insert(std::make_pair(_source_file_uri, local_file));
                return true;
                break;
        }
    } catch (...) {
    }
    return false;
}

bool abstracted_file_io::delete_file(const std::string &_source_file_uri) {
    try {
        std::string source_location = uri_to_file(_source_file_uri);
        sca_file_struct sca_file;
        local_file_struct local_file;
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_file_manager->remove(source_location.c_str());
                return true;
                break;
            case LOCAL_FILESYSTEM:
                remove(source_location.c_str());
                return true;
                break;
        }
    } catch (...) {
    }
    return false;
}

bool abstracted_file_io::file_seek(const std::string &_source_file_uri, unsigned long long offset_from_beginning) {
    try {
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_file_mapping.find(_source_file_uri)->second.curFile->setFilePointer(offset_from_beginning);
                return true;
                break;
            case LOCAL_FILESYSTEM:
                local_file_mapping.find(_source_file_uri)->second.curFile->seekp(offset_from_beginning, std::ios::beg);
                local_file_mapping.find(_source_file_uri)->second.curFile->seekg(offset_from_beginning, std::ios::beg);
                return true;
                break;
        }
    } catch (...) {
    }
    return false;


}

unsigned long long abstracted_file_io::file_tell(const std::string &_source_file_uri) {
    try {
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                return sca_file_mapping.find(_source_file_uri)->second.curFile->filePointer();
                break;
            case LOCAL_FILESYSTEM:
                return local_file_mapping.find(_source_file_uri)->second.curFile->tellg();
                break;
        }
    } catch (...) {
    }
    return -1;
}

unsigned long long abstracted_file_io::file_size(const std::string &_source_file_uri) {
    try {
        std::string non_uri_source = uri_to_file(_source_file_uri);
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                return sca_file_mapping.find(_source_file_uri)->second.curFile->sizeOf();
                break;
            case LOCAL_FILESYSTEM:
                return boost::filesystem::file_size(BOOST_FILESYSTEM_PATH(non_uri_source));
                break;
        }
    } catch (...) {
    }
    return 0;
}


bool abstracted_file_io::close_file(const std::string &_source_file_uri) {
    try {
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_file_mapping.find(_source_file_uri)->second.curFile->close();
                sca_file_mapping.find(_source_file_uri)->second.curFile = CF::File::_nil();
                sca_file_mapping.erase(sca_file_mapping.find(_source_file_uri));
                return true;
                break;
            case LOCAL_FILESYSTEM:
                local_file_mapping.find(_source_file_uri)->second.curFile->close();
                delete local_file_mapping.find(_source_file_uri)->second.curFile;
                local_file_mapping.erase(local_file_mapping.find(_source_file_uri));
                return true;
                break;
        }
    } catch (...) {
    }
    return false;
}

bool abstracted_file_io::move_file(const std::string &_source_file_uri, const std::string &_dest_file_uri) {
    try {
        std::string non_uri_source = uri_to_file(_source_file_uri);
        std::string non_uri_dest = uri_to_file(_dest_file_uri);
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_file_manager->copy(non_uri_source.c_str(), non_uri_dest.c_str());
                sca_file_manager->remove(non_uri_source.c_str());
                return true;
                break;
            case LOCAL_FILESYSTEM:
                if (rename(non_uri_source.c_str(), non_uri_dest.c_str()) != 0)
                    return false;
                return true;
                break;
        }
    } catch (...) {
    }
    return false;
}

bool abstracted_file_io::copy_file(const std::string &_source_file_uri, const std::string &_dest_file_uri) {
    try {
        std::string non_uri_source = uri_to_file(_source_file_uri);
        std::string non_uri_dest = uri_to_file(_dest_file_uri);
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_file_manager->copy(non_uri_source.c_str(), non_uri_dest.c_str());
                return true;
                break;
            case LOCAL_FILESYSTEM:
            	boost::filesystem::copy_file(non_uri_source,non_uri_dest);
            	if (!boost::filesystem::exists(non_uri_dest)) {
            		return false;
            	}
            	return true;
                break;
        }
    } catch (...) {
    }
    return false;
}

bool abstracted_file_io::read(const std::string &_source_file_uri, std::vector<char> * dataBuffer, size_t size_bytes) {
    try {
        std::map<std::string, sca_file_struct >::iterator sca_fs_iter;
        std::map<std::string, local_file_struct >::iterator local_fs_iter;
        size_t data_length = 0;
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_fs_iter = sca_file_mapping.find(_source_file_uri);
                sca_fs_iter->second.curFile->read(sca_fs_iter->second.buffer, size_bytes);
                data_length = sca_fs_iter->second.buffer->length();
                VECTOR_MAGIC_HELPERS::std_vector_instant_resize(dataBuffer, data_length);
                if (data_length > 0)
                    memcpy(& (*dataBuffer)[0], &sca_fs_iter->second.buffer[0], data_length);
                return (data_length == size_bytes);
                break;
            case LOCAL_FILESYSTEM:
                local_fs_iter = local_file_mapping.find(_source_file_uri);
                VECTOR_MAGIC_HELPERS::std_vector_instant_resize(dataBuffer, size_bytes);
                local_fs_iter->second.curFile->read(& (*dataBuffer)[0], size_bytes);
                data_length = local_fs_iter->second.curFile->gcount();
                VECTOR_MAGIC_HELPERS::std_vector_instant_resize(dataBuffer, data_length);
                return !local_fs_iter->second.curFile->eof();
                break;
        }
    } catch (...) {
        std::cerr << "EXCEPTION CAUGHT DURING READ!\n";
    }
    return true;


}

bool abstracted_file_io::write(const std::string &_source_file_uri, char * dataBuffer, size_t dataLength, bool forceFlush) {
    try {
        std::map<std::string, sca_file_struct >::iterator sca_fs_iter;
        std::map<std::string, local_file_struct >::iterator local_fs_iter;
        switch (source_uri_to_filesystem(_source_file_uri)) {
            case SCA_FILESYSTEM:
                sca_fs_iter = sca_file_mapping.find(_source_file_uri);
                sca_fs_iter->second.curFile->write(CF::OctetSequence(dataLength, dataLength, (CORBA::Octet*) dataBuffer, false));
                return true;
                break;
            case LOCAL_FILESYSTEM:
                local_fs_iter = local_file_mapping.find(_source_file_uri);
                local_fs_iter->second.curFile->write(dataBuffer, dataLength);
                if (forceFlush)
                    local_fs_iter->second.curFile->flush();
                return true;
                break;
        }
    } catch (...) {
        std::cerr << "EXCEPTION CAUGHT DURING WRITE!\n";
        return false;
    }
    return true;


}

bool abstracted_file_io::make_dir(const std::string &_source_dir_uri) {
    try {
        std::map<std::string, sca_file_struct >::iterator sca_fs_iter;
        std::map<std::string, local_file_struct >::iterator local_fs_iter;
        std::string source_location = uri_to_file(_source_dir_uri);
        switch (source_uri_to_filesystem(_source_dir_uri)) {
            case SCA_FILESYSTEM:
                try{
                sca_file_manager->mkdir(source_location.c_str());
                } catch(...){};
                return sca_file_manager->exists(source_location.c_str());
            case LOCAL_FILESYSTEM:
                boost::filesystem::create_directories(BOOST_FILESYSTEM_PATH(source_location));
                return boost::filesystem::is_directory(source_location);
                break;
        }
    } catch (...) {
        std::cerr << "EXCEPTION CAUGHT DURING MAKE DIRECTORY!\n";
        return false;
    }
    return true;


}

bool abstracted_file_io::uri_path_extraction(const std::string& uri_full, std::string& dirname, std::string& basename, FILESYSTEM_TYPE& file_sys_type, bool force_as_dir) {
    // File System Determination
    file_sys_type = LOCAL_FILESYSTEM;
    if (uri_full.find(sca_uri_prefix) == 0)
        file_sys_type = SCA_FILESYSTEM;
    
    // Remove URI
    std::string nonuri_full = uri_full;
    if (file_sys_type == SCA_FILESYSTEM)
        nonuri_full = replace_string(nonuri_full, sca_uri_prefix, "");
    else
        nonuri_full = replace_string(nonuri_full, local_uri_prefix, "");

    if (force_as_dir)
        nonuri_full += '/';

    //Determine Dir and Base
    boost::filesystem::path full_path = BOOST_FILESYSTEM_PATH(nonuri_full);
    full_path.normalize();
    basename = BOOST_PATH_STRING(full_path.filename());
    dirname = full_path.string();
    if (!basename.empty()) {
        size_t pos = dirname.rfind(basename);
        dirname.erase(pos);
    }
    return true;
}

std::string abstracted_file_io::normalize_uri_path(const std::string& uri_full) {
    std::string normalized_uri = uri_full;
    if (normalized_uri.find(sca_uri_prefix) != 0 && normalized_uri.find(local_uri_prefix))
        normalized_uri = local_uri_prefix + normalized_uri;
    std::string prefix, dir, base;
    FILESYSTEM_TYPE type;
    uri_path_extraction(normalized_uri, dir, base, type);
    prefix = local_uri_prefix;
    if (type == SCA_FILESYSTEM)
        prefix = sca_uri_prefix;
    return prefix + dir + base;

}


bool abstracted_file_io::file_to_sca_file(const std::string& _source_file_uri, const std::string& _sca_file_uri ){
	try {
		std::string source_location = uri_to_file(_source_file_uri);
		std::string dest_location = uri_to_file(_sca_file_uri);

		switch (source_uri_to_filesystem(_source_file_uri)) {
		case SCA_FILESYSTEM:
			try{
				sca_file_manager->copy(source_location.c_str(), dest_location.c_str());
			} catch(...){};
			return sca_file_manager->exists(dest_location.c_str());
			break;
		case LOCAL_FILESYSTEM:
			local_file_struct source_file;
			CF::File_var dest_file;

			//Open Source File
			std::ios_base::openmode mode = std::ios::in;
			source_file.curFile = new std::fstream();
			source_file.curFile->open(source_location.c_str(), mode);
			if (!source_file.curFile->is_open()) {
				delete source_file.curFile;
				source_file.curFile = NULL;
				return false;

			}

			//Open Dest File
			try {
				dest_file = sca_file_manager->open(dest_location.c_str(), false);
			}catch (...) {
				sca_file_manager->create(dest_location.c_str());
				dest_file = sca_file_manager->open(dest_location.c_str(),false);
			}
			if (CORBA::is_nil(dest_file)) {
				return false;
			}

			// write to file
			size_t size_bytes(BUFF_MAX);
			size_t data_length(0);
			VECTOR_MAGIC_HELPERS::std_vector_instant_resize(& source_file.buffer,size_bytes);
			bool eof = false;
			while(!eof){
				source_file.curFile->read(& source_file.buffer[0], size_bytes);
				data_length = source_file.curFile->gcount();
				dest_file->write(CF::OctetSequence(data_length, data_length, (CORBA::Octet*) &source_file.buffer[0], false));
				eof = source_file.curFile->eof();
			}

			dest_file->close();
			source_file.curFile->close();
			delete source_file.curFile;
			return true;
			break;
		}

	} catch (...) {
		std::cerr << "EXCEPTION CAUGHT DURING READ!\n";
		return false;
	}
	return true;


}

