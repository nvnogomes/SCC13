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

    std::map<std::string, float>::iterator iter = this->histogram_.begin();
    char current_letter = 'A';
    std::ostringstream buf ("");
    std::ostringstream letter_buffer ("");
    buf << current_letter << std::endl;


    for (; iter != this->histogram_.end(); ++iter) {

        if( toupper(iter->first[0]) != current_letter ) {

            this->letters_[current_letter] = letter_buffer.str();
            letter_buffer.str("");
            letter_buffer.clear();

            current_letter = toupper(iter->first[0]);
            buf << current_letter << std::endl;
        }

        letter_buffer << iter->first << " " << iter->second << " ";
        letter_buffer << this->get_filename() << std::endl;

        float word_permil = (iter->second / this->histogram_.size()) * 10;
        buf << iter->first << " ";
        buf << iter->second << " " << word_permil << std::endl;
    }
    this->letters_[toupper(letter_buffer.str()[0])] = letter_buffer.str();


    this->output_string_ = buf.str();
}


/**
 * @brief Indexer::get_filename
 * @return
 */
std::string Indexer::get_filename() {
    int file_index = this->filename_.rfind("/") + 1;
    int str_lenght = this->filename_.size() - file_index;
    return this->filename_.substr( file_index, str_lenght );
}


/**
 * @brief Indexer::build_output_filename
 * @return
 */
std::string Indexer::build_output_filename() {

    return this->directory_ +""+ this->get_filename() +".index";
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

    // document index
    std::string output_filename = build_output_filename();
    std::ofstream output_stream_file;
    output_stream_file.open( output_filename.c_str(), std::ios::out );
    output_stream_file << this->output_string_;
    output_stream_file.close();

    // letter index
    std::map<char,std::string>::iterator it;
    for( it = this->letters_.begin(); it != this->letters_.end(); it++) {
        std::string letter_filename = this->directory_ +""+ it->first +""+ ".index";
        std::ofstream letter_output_file;
        letter_output_file.open( letter_filename.c_str(), std::ios::out | std::ios::app);
        letter_output_file << it->second;
        letter_output_file.close();
    }
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
            std::transform(word.begin(), word.end(), word.begin(), toupper);
            word = this->filter( word );
            if( word.length() > 1 ) {
                this->histogram_[ word ]++;
            }
        }
        input.close();
    }
    else {
        std::cerr << "Error: Could not open file." << std::endl;
    }
}


/**
 * @brief Indexer::filter
 * @param word
 * @return
 */
std::string Indexer::filter(std::string word) {

    if( this->start_match(word, "WWW") || this->start_match(word, "HTTP")) {
        return "";
    }

    return this->trim_word(word);
}


/**
 * @brief Indexer::trimWord removes non alphanumeric characters
 * from the begining and end of the word
 * @param word to be trimmed
 * @return trimmed word
 */
std::string Indexer::trim_word( std::string word) {

    if( word.empty() )
        return "";

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



/**
 * @brief Indexer::start_match
 * @param str
 * @param match
 * @return
 */
bool Indexer::start_match(std::string str, std::string match) {
    return str.substr(0, match.size()) == match;
}


