#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP

#include "Token.hpp"
#include <string_view>
#include <memory>

class TokenStream
{
public:
    std::shared_ptr<Token> GetToken();
    std::shared_ptr<Token> Peek();
    std::shared_ptr<Token> ReadTokenFromInput();
    void PushBack(std::shared_ptr<Token> t);
    void Ignore(char c); // Discard the characters up to and including the argument "c."

private:
    bool full = false;
    bool has_lookahead = false;
    std::shared_ptr<Token> buffer;
    std::shared_ptr<Token> lookahead;
};

#endif