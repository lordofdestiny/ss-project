%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
    #include <cstdio>
    #include <iostream>
    #include <iomanip>
    #include <iostream>
    #include <vector>
    #include <memory>
    #include <string>
    #include <utility>
    #include <algorithm>
    #include <iterator>
    #include <variant>
    #include <unordered_map>
    #include <cstdint>
    #include <optional>
    #include "parser_objects.h"
    #include "ast/stmt_t.h"

    namespace m_asm{
        class parser_driver;
    }

    using m_asm::ast::stmt_t;
    using instr_t = m_asm::ast::stmt_t::instr_t;
    using m_asm::ast::expr_t;
    using namespace m_asm::ast::operand;
    using op_type_t = m_asm::ast::operand::operand_t::type_t;
    using word_arg_t = m_asm::ast::word_argument_t;
    using word_args_t = m_asm::ast::word_arguments_t;
    using mnemonic_t = m_asm::ast::stmt_t::instr_t::mnemonic_t;
    using utility::str_vec_t;
}

%param {m_asm::parser_driver& driver}

%locations

%define api.namespace {m_asm}
%define api.parser.class {bison_parser}
%define parse.trace
%define parse.error verbose
%define parse.lac full

%code {
    #include "location.hh"
    #include "parser_driver.h"
}


%token ENDL

// Literal tokens
%token
    COMMA           ","
    COLON           ":"
    PLUS            "+"
    MINUS           "-"
    LEFT_BRACKET    "["
    RIGHT_BRACKET   "]"
    DOLLAR_SIGN     "$"
;

// directive tokens
%token GLOBAL EXTERN
%token SECTION WORD
%token SKIP ASCII
%token EQUALS

// instruction tokens
%token INST_BEQ INST_BNE INST_BGT
%token <mnemonic_t> INST_COND_JUMP

%token INST_CALL INST_JMP
%token <mnemonic_t> INST_UNCOND_JUMP

%token INST_HALT INST_INT INST_IRET INST_RET
%token <mnemonic_t> INST_NO_PARAMS

%token INST_PUSH INST_POP INST_NOT
%token <mnemonic_t> INST_ONE_PARAM

%token INST_XCHG INST_ADD INST_SUB INST_MUL INST_DIV
%token INST_AND INST_OR INST_XOR INST_SHL INST_SHR
%token <mnemonic_t> INST_TWO_PARAMS

%token <mnemonic_t> INST_LD
%token <mnemonic_t> INST_ST
%token <mnemonic_t> INST_CSRRD
%token <mnemonic_t> INST_CSRWR

%token <uint8_t> REG
%token <uint8_t> CREG

// literals and symbols tokens
%token <uint32_t> UINT_LITERAL
%token <std::string> STR_LITERAL
%token <std::string> SYMBOL

%type <str_vec_t> symbols
%type <word_arg_t> word_argument
%type <word_args_t> word_arguments

%type <std::unique_ptr<expr_t>> expression
%type <std::unique_ptr<expr_t>> expression_operand
%type <std::unique_ptr<expr_t>> expression_primary
%type <char> expression_operator

%type <std::unique_ptr<stmt_t::label_t>> label
%type <std::unique_ptr<stmt_t::global_t>> global
%type <std::unique_ptr<stmt_t::extern_t>> extern
%type <std::unique_ptr<stmt_t::section_t>> section
%type <std::unique_ptr<stmt_t::word_t>> word
%type <std::unique_ptr<stmt_t::skip_t>> skip
%type <std::unique_ptr<stmt_t::ascii_t>> ascii
%type <std::unique_ptr<stmt_t::equals_t>> equals

%type <operand_t> operand
%type <operand_t> jmp_operand

%type <line_content_t> directive
%type <line_content_t> instruction
%type <line_content_t> line_content
%type <line_content_t> line
%type <parsed_file_t> lines

// %printer { yyo << $$; } <*>;

%%
assembly_file:
    OPT_ENDLS lines {
        driver.parsed_file = std::move($2);
    }
lines:
    lines line {
        $1.emplace_back(std::move($2));
        $$ = std::move($1);
    }
    | line {
         $$.emplace_back(std::move($1));
    };
line:
    line_content OPT_ENDLS {
        $$ = std::move($1);
    }
line_content:
    label {
        $$ = std::move($1);
    }
    | directive {
        $$ = std::move($1);
    }
    | instruction {
        $$ = std::move($1);
    }
label:
    SYMBOL ":" {
        $$ = std::make_unique<stmt_t::label_t>(std::move($1));
    }
symbols:
    symbols "," SYMBOL {
        $$ = std::move($1);
        $$.emplace_back(std::move($3));
    } | SYMBOL {
        $$.emplace_back(std::move($1));
    };
directive:
    global {
        $$ = std::move($1);
    }
    | extern {
        $$ = std::move($1);
    }
    | section {
        $$ = std::move($1);
    }
    | word {
        $$ = std::move($1);
    }
    | skip {
        $$ = std::move($1);
    }
    | ascii {
        $$ = std::move($1);
    }
    | equals {
        $$ = std::move($1);
    }
global:
    GLOBAL symbols {
        $$ = std::make_unique<stmt_t::global_t>(
                std::move($2)
        );
    };
extern:
    EXTERN symbols {
        $$ = std::make_unique<stmt_t::extern_t>(
                std::move($2)
        );
    };
section:
    SECTION SYMBOL {
        $$ = std::make_unique<stmt_t::section_t>(
                std::move($2)
        );
    };
word:
    WORD word_arguments  {
        $$ = std::make_unique<stmt_t::word_t>(
                std::move($2)
        );
    };
skip:
    SKIP UINT_LITERAL {
        $$ = std::make_unique<stmt_t::skip_t>(
                std::move($2)
        );
    }
ascii:
    ASCII STR_LITERAL {
        $$ = std::make_unique<stmt_t::ascii_t>(
                std::move($2)
        );
    };
equals:
    EQUALS SYMBOL "," expression {
        $$ = std::make_unique<stmt_t::equals_t>(
                std::move($2),
                std::move($4)
        );
    };
word_arguments:
    word_arguments "," word_argument {
        $$ = std::move($1);
        $$.emplace_back(std::move($3));
    } | word_argument {
        $$.emplace_back(std::move($1));
    };
word_argument:
    UINT_LITERAL {
        $$ = $1;
    }
    | SYMBOL {
        $$ =  $1;
    }
    | expression_operator UINT_LITERAL {
       $$ = static_cast<uint32_t>(('-' - $1 - 1) * $2);
    }
expression:
    expression expression_operator expression_operand {
        $$ = std::make_unique<expr_t::sum_t>(
                std::move($1),
                $2,
                std::move($3)
            );
    }| expression_operand {
        $$ = std::move($1);
    };
expression_operand:
    expression_operator expression_primary {
        $$ = std::make_unique<expr_t::unary_t>(
                $1, std::move($2)
            );
    } | expression_primary {
        $$ = std::move($1);
    }
expression_operator:
    PLUS {
        $$ = '+';
    }
    | MINUS {
        $$ = '-';
    }
expression_primary:
    UINT_LITERAL {
        $$ = std::make_unique<expr_t::int_literal_t>($1);
    } | SYMBOL {
        $$ = std::make_unique<expr_t::symbol_t>(std::move($1));
    };
instruction:
    INST_NO_PARAMS {
        $$ = std::make_unique<instr_t::paramless_t>(
                $1
            );
    }
    | INST_UNCOND_JUMP jmp_operand {
        $$ = std::make_unique<instr_t::uncond_t>(
                $1, std::move($2)
            );
    }
    | INST_COND_JUMP REG "," REG "," jmp_operand {
        $$ = std::make_unique<instr_t::branch_t>(
                $1, $2, $4, std::move($6)
            );
    }
    | INST_ONE_PARAM REG {
        $$ = std::make_unique<instr_t::unary_t>(
                $1, $2
            );
    }
    | INST_TWO_PARAMS REG "," REG {
       $$ = std::make_unique<instr_t::binary_t>(
                $1, $2, $4
            );
    }
    | INST_LD operand "," REG {
         $$ = std::make_unique<instr_t::memory_t>(
                $1, std::move($2), $4
            );
    }
    | INST_ST REG "," operand {
        $$ = std::make_unique<instr_t::memory_t>(
                $1, $2, std::move($4)
            );
    }
    | INST_CSRRD CREG "," REG {
        $$ = std::make_unique<instr_t::binary_t>(
                $1, $2, $4
            );
    }
    | INST_CSRWR REG "," CREG {
    	$$ = std::make_unique<instr_t::binary_t>(
                $1, $2, $4
            );
    };
jmp_operand:
    UINT_LITERAL {
        $$.type = op_type_t::LITERAL_VALUE;
    	$$.value = $1;
    }
    | SYMBOL {
        $$.type = op_type_t::SYMBOL_VALUE;
    	$$.value = $1;
    }
operand:
    "$" UINT_LITERAL {
	    $$.type = op_type_t::LITERAL_VALUE;
    	$$.value = $2;
    }
    | "$" SYMBOL {
    	$$.type = op_type_t::SYMBOL_VALUE;
    	$$.value = $2;
    }
    | UINT_LITERAL {
    	$$.type = op_type_t::LITERAL_ADDR;
    	$$.value = $1;
    }
    | SYMBOL {
    	$$.type = op_type_t::SYMBOL_ADDR;
    	$$.value = $1;
    }
    | REG{
    	$$.type = op_type_t::REG_VALUE;
    	$$.value = $1;
    }
    | "[" REG "]" {
    	$$.type = op_type_t::REG_MEMORY_ADDR;
    	$$.value = $2;
    }
    | "[" REG "+" UINT_LITERAL "]" {
        $$.type = op_type_t::REG_ADDR_LITERAL_OFFSET;
    	$$.value = std::make_tuple($2, $4);
    }
    | "[" REG "-" UINT_LITERAL "]" {
        $$.type = op_type_t::REG_ADDR_LITERAL_OFFSET;
    	$$.value = std::make_tuple($2, static_cast<uint32_t>(-$4));
    }
    | "[" REG "+" SYMBOL "]" {
        $$.type = op_type_t::REG_ADDR_SYMBOL_OFFSET;
    	$$.value = std::make_tuple($2, $4);
    };
ENDLS:
    ENDLS ENDL | ENDL;
OPT_ENDLS:
    ENDLS | /* empty */;
%%

namespace m_asm {
    void bison_parser::error(const location_type& l, std::string const& m) {
        driver.parsed_file = parse_error_t{
            {}, m, l.begin.line, l.begin.column
        };
    }
}
