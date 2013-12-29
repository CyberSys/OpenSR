/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RANGERS_QM_PARSER_H
#define RANGERS_QM_PARSER_H

#include <list>
#include <map>

#include "QM.h"

namespace Rangers
{
namespace QM
{
//FIXME: better way for token abstraction?
class QM_API Token
{

public:
    enum Type {TOKEN_NONE = 255, TOKEN_NUMBER = 0, TOKEN_RANGE = 1, TOKEN_OPEN_PAR = 2, TOKEN_CLOSE_PAR = 3, TOKEN_OP_PLUS = 4,
               TOKEN_OP_MINUS = 5, TOKEN_OP_DIV = 6, TOKEN_OP_MULTIPLY = 7, TOKEN_OP_MOD = 8, TOKEN_EQUAL = 9,
               TOKEN_MORE = 10, TOKEN_LESS = 11, TOKEN_MORE_EQUAL = 12, TOKEN_LESS_EQUAL = 13, TOKEN_NOT_EQUAL = 14,
               TOKEN_OR = 15, TOKEN_AND = 16, TOKEN_NOT = 17, TOKEN_TO = 18, TOKEN_IN = 19, TOKEN_PARAMETER = 20,
               TOKEN_LIST = 21
              };
    Token();
    Token(float number);
    Token(float from, float to);

    Type type;

    union
    {
        uint32_t id;
        float number;
        struct
        {
            float from;
            float to;
        };
    } value;

    std::vector<float> list;

    Token apply(const Token &a, const Token &b, const std::map<uint32_t, float> &parameters) const;
};

QM_API std::list<Token> tokenize(const std::string& exp);
QM_API float eval(const std::list<Token>& exp, const std::map<uint32_t, float> &parameters);
QM_API float eval(const std::string& exp, const std::map<uint32_t, float> &parameters);
}
}

#endif
