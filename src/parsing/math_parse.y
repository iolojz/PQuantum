%{
#include "parsing/config.hpp"
// #include things that define yylex
// #include things that define yyerror
%}

%require "3.2"
%language "c++"

%define api.namespace {PQuantum::parsing::bison}
%define api.value.type variant
%define api.token.constructor

%lex-param {yyscan_t scanner}

%token <PQuantum::parsing::number> NUMBER
%token <PQuantum::parsing::identifier> IDENTIFIER
%token <PQuantum::parsing::index> INDEX
%nterm <PQuantum::parsing::index_list> index_list
%nterm <PQuantum::parsing::index_spec> index_spec
%nterm <PQuantum::parsing::identifier_with_optional_indices> identifier_with_optional_indices
%nterm <PQuantum::parsing::argument_list> argument_list
%nterm <PQuantum::parsing::function_call> function_call
%nterm <PQuantum::parsing::arithmetic_expression> arithmetic_expr
%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'

%%

index_list:
  %empty                                         {}
| INDEX ',' index_list                           { $$.push_back($1); $$.insert( std::end($$), std::begin($3), std::end($3) ); }
;

index_spec:
  '_' '{' index_list '}' '^' '{' index_list '}'  { $$.lower = $3; $$.upper = $7; }
| '^' '{' index_list '}' '_' '{' index_list '}'  { $$.upper = $3; $$.lower = $7; }
| '_' '{' index_list '}'                         { $$.lower = $3; }
| '^' '{' index_list '}'                         { $$.lower = $3; }
;

identifier_with_optional_indices:
  IDENTIFIER index_spec                          { $$.name = $1; $$.indices = $1; }
| IDENTIFIER                                     { $$.name = $1; }
;

argument_list:
  %empty                                         {}
| arithmetic_expr ',' argument_list              { $$.push_back($1); $$.insert( std::end($$), std::begin($3), std::end($3) ); }
;

function_call:
  IDENTIFIER '{' argument_list '}'               { $$.name = $1; $$.arguments = $3 }
;

arithmetic_expr:
  NUMBER                                         { $$ = $1; }
| identifier_with_optional_indices               { $$ = $1; }
| function_call                                  { $$ = $1; }
| arithmetic_expr '+' arithmetic_expr            { $$ = PQuantum::parsing::binary_plus{ $1, $3 }; }
| arithmetic_expr '-' arithmetic_expr            { $$ = PQuantum::parsing::binary_minus{ $1, $3 }; }
| arithmetic_expr '*' arithmetic_expr            { $$ = PQuantum::parsing::binary_multiply{ $1, $3 }; }
| arithmetic_expr '/' arithmetic_expr            { $$ = PQuantum::parsing::binary_divide{ $1, $3 }; }
| '-' arithmetic_expr  %prec NEG                 { $$ = PQuantum::parsing::unary_minus{ $2 }; }
| arithmetic_expr '^' arithmetic_expr            { $$ = PQuantum::parsing::binary_power{ $1, $3 }; }
| '(' arithmetic_expr ')'                        { $$ = $2; }
;

%%
