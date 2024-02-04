%{ /* -*- C++ -*- */
    #include <cerrno>
    #include <cstdlib>
    #include <cstring>
    #include <string>
    #include <vector>
    #include "parser.hh"
    #include "parser_driver.h"
    #include "parser_objects.h"

    using namespace std;
    using parser = m_asm::bison_parser;
    using mnemonic_t = m_asm::ast::stmt_t::instr_t::mnemonic_t;

    #define YY_DECL m_asm::bison_parser::symbol_type yylex (m_asm::parser_driver& driver)

%}

%option noyywrap nounput noinput batch debug

blank           [ \t\r]
dec_int         0|([1-9][0-9]*)
oct_int         0[0-7]+
hex_int         0(x|X)[0-9a-fA-F]+
integer         {dec_int}|{oct_int}|{hex_int}
symbol          [a-zA-Z][a-zA-Z0-9_\-]*
string          \".*\"
register        %r([0-9]|1[0-5])

%x COMMENT
%x END
%x STR

%{
    # define YY_USER_ACTION loc.columns (yyleng);
%}
%%
    using namespace std::string_literals;
    std::vector<char> string_buff;

%{
    m_asm::location& loc = driver.loc;
    loc.step();
%}

<INITIAL>\n+        loc.lines(yyleng); return parser::make_ENDL(loc);

#                   BEGIN(COMMENT);
<COMMENT>.+          ;// ignore comments
<COMMENT>\n         loc.lines(yyleng); BEGIN(INITIAL); return parser::make_ENDL(loc);

<END>.|\n           ;


{blank}+            loc.step(); // ignore whitespace
","                 return parser::make_COMMA(loc);
":"                 return parser::make_COLON(loc);

\.global            return parser::make_GLOBAL(loc);
\.extern            return parser::make_EXTERN(loc);
\.section           return parser::make_SECTION(loc);
\.word              return parser::make_WORD(loc);
\.skip              return parser::make_SKIP(loc);
\.ascii             return parser::make_ASCII(loc);
\.equ               return parser::make_EQUALS(loc);
\.end               BEGIN(END);


halt                return parser::make_INST_NO_PARAMS(mnemonic_t::HALT, loc);

int                 return parser::make_INST_NO_PARAMS(mnemonic_t::INT, loc);
iret                return parser::make_INST_NO_PARAMS(mnemonic_t::IRET, loc);

call                return parser::make_INST_UNCOND_JUMP(mnemonic_t::CALL, loc);
ret                 return parser::make_INST_NO_PARAMS(mnemonic_t::RET, loc);

jmp                 return parser::make_INST_UNCOND_JUMP(mnemonic_t::JMP, loc);
beq                 return parser::make_INST_COND_JUMP(mnemonic_t::BEQ, loc);
bne                 return parser::make_INST_COND_JUMP(mnemonic_t::BNE, loc);
bgt                 return parser::make_INST_COND_JUMP(mnemonic_t::BGT, loc);

push                return parser::make_INST_UNARY(mnemonic_t::PUSH, loc);
pop                 return parser::make_INST_UNARY(mnemonic_t::POP, loc);

add                 return parser::make_INST_BINARY(mnemonic_t::ADD, loc);
sub                 return parser::make_INST_BINARY(mnemonic_t::SUB, loc);
mul                 return parser::make_INST_BINARY(mnemonic_t::MUL, loc);
div                 return parser::make_INST_BINARY(mnemonic_t::DIV, loc);

not                 return parser::make_INST_UNARY(mnemonic_t::NOT, loc);
and                 return parser::make_INST_BINARY(mnemonic_t::AND, loc);
or                  return parser::make_INST_BINARY(mnemonic_t::OR, loc);
xor                 return parser::make_INST_BINARY(mnemonic_t::XOR, loc);

shl                 return parser::make_INST_BINARY(mnemonic_t::SHL, loc);
shr                 return parser::make_INST_BINARY(mnemonic_t::SHR, loc);

ld                  return parser::make_INST_LD(mnemonic_t::LD, loc);
st                  return parser::make_INST_ST(mnemonic_t::ST, loc);

xchg                return parser::make_INST_BINARY(mnemonic_t::XCHG, loc);
csrrd               return parser::make_INST_CSRRD(mnemonic_t::CSRRD, loc);
csrwr               return parser::make_INST_CSRWR(mnemonic_t::CSRWR, loc);

{register}          return parser::make_REG(atoi(yytext+2), loc);
%sp                 return parser::make_REG(14, loc);
%pc                 return parser::make_REG(15, loc);
%status             return parser::make_CREG(16 + 0, loc);
%handler            return parser::make_CREG(16 + 1, loc);
%cause              return parser::make_CREG(16 + 2, loc);

"+"                 return parser::make_PLUS(loc);
"-"                 return parser::make_MINUS(loc);
"["                 return parser::make_LEFT_BRACKET(loc);
"]"                 return parser::make_RIGHT_BRACKET(loc);
"$"                 return parser::make_DOLLAR_SIGN(loc);


\"                  string_buff.clear(); BEGIN(STR);
<STR>{
      \"             {
                        BEGIN(INITIAL);
                        return parser::make_STR_LITERAL(
                            {
                                string_buff.begin(),
                                string_buff.end()
                            }, loc);
                    }
      \n            throw parser::syntax_error(loc, "unterminated string literal");

      \\[0-7]{1,3}  {
                        int result = std::stoi(yytext+1, nullptr, 8);
                        if ( result > 0xFF ) {
                            throw parser::syntax_error(loc, "octal constant out of bounds");
                        }
                        string_buff.push_back(static_cast<char>(result & 0xFF));
                    }
      \\[0-9]+      {
                        throw parser::syntax_error(loc, "bad escape sequence; invalid octal constant");
                    }
      \\\\          string_buff.push_back('\\');
      \\n           string_buff.push_back('\n');
      \\t           string_buff.push_back('\t');
      \\r           string_buff.push_back('\r');
      \\b           string_buff.push_back('\b');
      \\v           string_buff.push_back('\v');
      \\f           string_buff.push_back('\a');

      \\(.|\n)      string_buff.push_back('\f');

      [^\\\n\"]+    {
                        char *yptr = yytext;
                        while (*yptr) string_buff.push_back(*yptr++);
                    }
}


{dec_int}           return parser::make_UINT_LITERAL(std::stoul(yytext, nullptr, 10), loc);
{oct_int}           return parser::make_UINT_LITERAL(std::stoul(yytext, nullptr, 8), loc);
{hex_int}           return parser::make_UINT_LITERAL(std::stoul(yytext, nullptr, 16), loc);
{symbol}            return parser::make_SYMBOL(yytext, loc);

.                   throw parser::syntax_error(loc, "invalid character: "s + yytext);

<<EOF>>             return parser::make_YYEOF (loc);
%%

namespace m_asm {
    bison_parser::symbol_type yylex (parser_driver& driver) {
        return ::yylex(driver);
    }
}