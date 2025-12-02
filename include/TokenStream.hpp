#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP

#include "Token.hpp"

class TokenStream {
public:
    Token* GetToken();
    void PushBack(Token* t);

private:
    bool full = false;
    Token* buffer;
};

#endif