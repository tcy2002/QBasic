#include "tokenizer.h"

#include <iostream>

const std::string Tokenizer::delim = " +-*/<>=()";

bool Tokenizer::isDelim(char c) {
    return (signed)delim.find(c) >= 0;
}

Tokenizer::Tokenizer(std::string str) {
    int size = str.size(), i, j;
    tokens.clear();

    for (i = 0; i < size;) {
        if (isDelim(str[i])) {
            if (str[i] != ' ') { // operators
                if (str[i] == '*' && i < size - 1 && str[i + 1] == '*') { // `**`
                    tokens.emplace_back(2, '*');
                    i += 2;
                } else { // other operators, including single `*`
                    tokens.emplace_back(1, str[i]);
                    i++;
                }
            } else { // space
                i++;
            }
        } else {
            for (j = i; j < size && !isDelim(str[j]); j++);
            tokens.emplace_back(str.begin() + i, str.begin() + j);
            i = j;
        }
    }
}

