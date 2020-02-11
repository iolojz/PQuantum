%{
#include "parsing/bison_types.hpp"
#include "parsing/math_scanner.hpp"
%}

%require "3.2"
%language "c++"

%define api.namespace {PQuantum::parsing::bison}
%define api.value.type variant
%define api.token.constructor

%lex-param {PQuantum::parsing::scanner_state& state}
%parse-param {PQuantum::parsing::scanner_state& state}

%token <atom> ATOM
%token <atom> INDEX
%nterm <index_list> index_list
%nterm <index_spec> index_spec
%nterm <identifier_with_optional_indices> identifier_with_optional_indices
%nterm <std::vector<arithmetic_expression>> argument_list
%nterm <function_call_node> function_call
%nterm <arithmetic_node> arithmetic_expr
%left '-' '+'
%left '/' '*'
%precedence NEG
%right '^'

%%

index_list:
  %empty                                         {}
| ATOM ',' index_list                            { $$.push_back(std::move($1)); $$.insert( std::end($$), std::make_move_iterator( std::begin($3) ), std::make_move_iterator( std::end($3) ) ); }
;

index_spec:
  '_' '{' index_list '}' '^' '{' index_list '}'  { $$.lower = std::move($3); $$.upper = std::move($7); }
| '^' '{' index_list '}' '_' '{' index_list '}'  { $$.upper = std::move($3); $$.lower = std::move($7); }
| '_' '{' index_list '}'                         { $$.lower = std::move($3); }
| '^' '{' index_list '}'                         { $$.lower = std::move($3); }
;

identifier_with_optional_indices:
  ATOM index_spec                                { $$.name = std::move($1); $$.indices = std::move($1); }
| ATOM                                           { $$.name = std::move($1); }
;

argument_list:
  %empty                                         {}
| arithmetic_expr ',' argument_list              { $$.push_back(std::move($1)); $$.insert( std::end($$), std::make_move_iterator( std::begin($3) ), std::make_move_iterator( std::end($3) ) ); }
;

function_call:
  ATOM '{' argument_list '}'                     { $$.data = std::move($1); $$.data.children = std::move($3) }
;

arithmetic_expr:
  ATOM                                           { $$ = std::move($1); }
| identifier_with_optional_indices               { $$ = std::move($1); }
| function_call                                  { $$ = std::move($1); }
| arithmetic_expr '+' arithmetic_expr            { $$ = make_arithmetic_sum( std::move($1), std::move($3) ); }
| arithmetic_expr '-' arithmetic_expr            { $$ = make_arithmetic_difference( std::move($1), std::move($3) ); }
| arithmetic_expr '*' arithmetic_expr            { $$ = make_arithmetic_product( std::move($1), std::move($3) ); }
| arithmetic_expr '/' arithmetic_expr            { $$ = make_arithmetic_quotient( std::move($1), std::move($3) ); }
| arithmetic_expr '^' arithmetic_expr            { $$ = make_arithmetic_power( std::move($1), std::move($3) ); }
| '-' arithmetic_expr  %prec NEG                 { $$ = make_arithmetic_negation( std::move($2) ); }
| '(' arithmetic_expr ')'                        { $$ = std::move($2); }
;

%%
