#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

#include "Merger.h"
#include "config.h"


/**
 * @brief Merger::Merger
 */
Merger::Merger():
    filename_( OUTPUT_DIRECTORY ),
    letter_map_()
{
    this->is_directory_ = true;
}


/**
 * @brief Merger::Merger
 * @param filename
 */
Merger::Merger(std::string filename):
    filename_(filename),
    letter_map_()
{
    if( this->is_file(filename.c_str()) ) {
        this->is_directory_ = false;
    }
    else {
        this->is_directory_ = true;
    }
}


/**
 * @brief Merger::~Merger
 */
Merger::~Merger() {}


/**
 * @brief Merger::merge
 * @param file
 */
void Merger::merge(std::string filename) {
    this->merge_file( filename );
    this->save( filename );
    letter_map_.clear();
}


/**
 * @brief Merger::run
 */
void Merger::run() {

    if( DEBUG ) {
        std::cout << "Running Merge on " << this->filename_ << std::endl;
    }

    std::list<std::string> files;
    if( this->is_directory_ ) {
        files = this->list_directory();
    }
    else {
        files.push_back( this->filename_ );
    }

    std::list<std::string>::iterator file_iterator = files.begin();
    for(; file_iterator != files.end(); file_iterator++ ) {

        this->merge( (*file_iterator) );
    }
}

/**
 * reads the file and sorts it by word and the documents where it exists
 *
 * @brief Merger::merge_file
 * @param filename
 */
void Merger::merge_file(std::string filename) {
    std::ifstream file_input( filename.c_str(), std::ios::in );

    std::string word;
    std::string occurences;
    std::string original_file;
    while (file_input >> word >> occurences >> original_file ) {
        this->letter_map_[word].push_back(occurences +" "+ original_file);
    }

    if( DEBUG ) {
        std::cout << filename << ": #" << this->letter_map_.size() << std::endl;
    }
}


/**
 * Save result to file
 *
 * @brief Merger::save
 * @param filename
 */
void Merger::save(std::string filename) {

    std::ofstream letter_output_file;
    letter_output_file.open( filename.c_str(), std::ios::out );

    std::map< std::string, std::list<std::string> >::iterator it;
    for( it = this->letter_map_.begin(); it != this->letter_map_.end(); it++) {

        letter_output_file << it->first << std::endl;
        std::list<std::string>::iterator entry;
        for (entry = it->second.begin(); entry != it->second.end(); ++entry) {
            letter_output_file << " " << (*entry) << std::endl;
        }
    }
    letter_output_file.close();
}


/**
 * List the given directory and returns the letter index files
 *
 * @brief Merger::list_directory
 * @return
 */
std::list<std::string> Merger::list_directory() {
    DIR *search_directory;
    struct dirent *entry;
    std::list<std::string> list;

    search_directory = opendir( this->filename_.c_str() );

    if( search_directory == NULL ) {
        printf("Error opening directory: %s\n", this->filename_.c_str());
        exit(2);
    }

    while( (entry = readdir( search_directory )) ){

        if( entry->d_type == DT_REG ){
            std::string filename = entry->d_name;
            if( filename.find( PARTIAL_EXTENSION ) != std::string::npos) {
                list.push_back( this->filename_ +""+ filename );
            }
        }
    }
    closedir( search_directory );

    return list;
}

/**
 * checks if the given path is a directory or a file
 *
 * @brief Merger::is_file
 * @param path
 * @return
 */
bool Merger::is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}
