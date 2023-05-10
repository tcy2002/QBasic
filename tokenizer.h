#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

/*
 * Type: token
 * -----------------
 * This type is used to define token.
 */

typedef std::string token;

/*
 * Class: Tokenizer
 * -----------------
 * This class is used to split strings into tokens.
 */

class Tokenizer {

public:

    /* some tools for parsing tokens */
    /* delimitations for a string, including operators */
    static const std::string delim;
    static inline bool isDelim(char c);

public:

    Tokenizer(std::string str);

    std::vector<token> tokens;

};

#endif // TOKENIZER_H
