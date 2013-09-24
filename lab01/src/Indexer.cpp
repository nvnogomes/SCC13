#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <locale>
#include <string>
#include <cctype>

#include "Indexer.h"


Indexer::Indexer(std::string filename, std::string output_directory):
    output_string_(""),
    histogram_()
{
    this->filename_= filename;
    this->directory_ = output_directory;

}


Indexer::~Indexer(){}


/**
 * @brief Indexer::buildMapString
 * Builds the string with the current histogram
 * to be printed on screen or written into a file
 */
void Indexer::build_histogram_string() {

    std::ostringstream buf ("");

    buf << this->histogram_.size() << std::endl;
    std::map<std::string, float>::iterator iter = this->histogram_.begin();
    char current_letter = 'k';
    for (; iter != this->histogram_.end(); ++iter) {
        if( toupper(iter->first[0]) != current_letter ) {
            current_letter = toupper(iter->first[0]);
            buf << current_letter << std::endl;
        }
        buf << iter->first << " " << iter->second << std::endl;
    }

    this->output_string_ = buf.str();
}


/**
 * @brief Indexer::build_output_filename
 * @return
 */
std::string Indexer::build_output_filename() {

    int file_index = this->filename_.rfind("/") + 1;
    int str_lenght = this->filename_.size() - file_index;
    std::string filename = this->filename_.substr( file_index, str_lenght );
    return this->directory_ +""+ filename +".index";
}


/**
 * @brief Indexer::histogram_size
 * @return
 */
int Indexer::histogram_size() {
    return this->histogram_.size();
}


/**
 * @brief Indexer::print
 */
void Indexer::print() {

    if( this->output_string_.empty() ) {
        build_histogram_string();
    }

    std::cout << this->output_string_ << std::endl;
}


/**
 * @brief Indexer::save
 */
void Indexer::save() {

    if( this->output_string_.empty() ) {
        build_histogram_string();
    }

    std::string output_filename = build_output_filename();
    std::ofstream output_stream_file;
    output_stream_file.open( output_filename.c_str(), std::ios::out );
    output_stream_file << this->output_string_;

    output_stream_file.close();
}


/**
 * @brief Indexer::run
 */
void Indexer::run() {

    std::ifstream input( this->filename_.c_str(), std::ios::in );

    if( input.is_open() ) {

        std::string word;

        while( input.good() ) {
            input >> word;
            word = trim_word( word );
            if( word.length() > 1 ) {
                std::transform(word.begin(), word.end(), word.begin(), toupper);
                this->histogram_[ word ]++;
            }
        }
        input.close();

        // histogram
        std::map<std::string, float>::iterator it;
        for(it = this->histogram_.begin(); it != this->histogram_.end(); it++) {
            float word_permil = (it->second / this->histogram_.size()) * 10;
            this->histogram_[it->first] = word_permil;
        }

    }
    else {
        std::cerr << "Error: Could not open file." << std::endl;
    }
}


/**
 * @brief Indexer::trimWord removes non alphanumeric characters
 * from the begining and end of the word
 * @param word to be trimmed
 * @return trimmed word
 */
std::string Indexer::trim_word( std::string word) {
    int index_begin = 0;
    while( !isalpha( word[index_begin] ) && index_begin < (int)word.size() ){
        index_begin++;
    }

    int index_end = word.size() - 1;
    while( !isalpha( word[index_end] ) && index_end > 0 ){
        index_end--;
    }

    int str_lenght = index_end - index_begin +1;

    if( str_lenght <= 0 ) {
        return "";
    }
    else {
        return word.substr(index_begin, str_lenght );
    }
}

