%{
#include "parsing/bison_types.hpp"
#include "parsing/math_scanner.hpp"
%}

%require "3.2"
%language "c++"

%define parse.trace

%define api.namespace {PQuantum::parsing::bison}
%define api.value.type variant
%define api.token.constructor
%locations
%define api.location.type {input_location}

%lex-param {scanner_state &state}
%parse-param {scanner_state &state} {math_tree &root}

%token <mathutils::string_atom> ATOM;
%token <mathutils::string_atom> INDEX;
%token PLUS;
%token MINUS;
%token ASTERISK;
%token SLASH;
%token CARET;
%token UNDERSCORE;
%token COMMA;
%token LEFT_ROUND_BRACE;
%token RIGHT_ROUND_BRACE;
%token LEFT_CURLY_BRACE;
%token RIGHT_CURLY_BRACE;
%token END_OF_INPUT 0;
%nterm <mathutils::index_list<mathutils::string_atom>> index_list;
%nterm <mathutils::index_spec<mathutils::string_atom>> index_spec;
%nterm <mathutils::atom_with_optional_indices<mathutils::string_atom>> atom_with_optional_indices;
%nterm <std::vector<math_tree>> argument_list;
%nterm <function_call_node> function_call;
%nterm <math_tree> arithmetic_expr;
%nterm <math_tree> root_arithmetic_expr;
%left PLUS
%left MINUS;
%left ASTERISK
%left SLASH;
%right CARET;

%start root_rule

%%

index_list:
  %empty                                           {}
| ATOM COMMA index_list                            { $$.push_back(std::move($1)); $$.insert( std::end($$), std::make_move_iterator( std::begin($3) ), std::make_move_iterator( std::end($3) ) ); }
| ATOM                                             { $$.push_back(std::move($1)); }
;

index_spec:
  UNDERSCORE LEFT_CURLY_BRACE index_list RIGHT_CURLY_BRACE CARET LEFT_CURLY_BRACE index_list RIGHT_CURLY_BRACE  { $$.lower = std::move($3); $$.upper = std::move($7); }
| CARET LEFT_CURLY_BRACE index_list RIGHT_CURLY_BRACE UNDERSCORE LEFT_CURLY_BRACE index_list RIGHT_CURLY_BRACE  { $$.upper = std::move($3); $$.lower = std::move($7); }
| UNDERSCORE LEFT_CURLY_BRACE index_list RIGHT_CURLY_BRACE                                                      { $$.lower = std::move($3); }
| CARET LEFT_CURLY_BRACE index_list RIGHT_CURLY_BRACE                                                           { $$.lower = std::move($3); }
;

atom_with_optional_indices:
  ATOM index_spec                                  { $$.atom = std::move($1); $$.indices = std::move($2); }
| ATOM                                             { $$.atom = std::move($1); }
;

argument_list:
  %empty                                           {}
| root_arithmetic_expr COMMA argument_list         { $$.push_back(std::move($1)); $$.insert( std::end($$), std::make_move_iterator( std::begin($3) ), std::make_move_iterator( std::end($3) ) ); }
| root_arithmetic_expr                             { $$.push_back(std::move($1)); }
;

function_call:
  ATOM LEFT_CURLY_BRACE argument_list RIGHT_CURLY_BRACE       { $$.data.atom = std::move($1.name); $$.children.insert( std::end($$.children), std::make_move_iterator( std::begin($3) ), std::make_move_iterator( std::end($3) ) ); }
;

arithmetic_expr:
  atom_with_optional_indices                               { $$ = std::move($1); }
| function_call                                            { $$ = std::move($1); }
| arithmetic_expr PLUS arithmetic_expr                     { $$ = make_arithmetic_sum( std::move($1), std::move($3) ); }
| arithmetic_expr MINUS arithmetic_expr                    { $$ = make_arithmetic_difference( std::move($1), std::move($3) ); }
| arithmetic_expr ASTERISK arithmetic_expr                 { $$ = make_arithmetic_product( std::move($1), std::move($3) ); }
| arithmetic_expr arithmetic_expr  %prec ASTERISK          { $$ = make_arithmetic_product( std::move($1), std::move($2) ); }
| arithmetic_expr SLASH arithmetic_expr                    { $$ = make_arithmetic_quotient( std::move($1), std::move($3) ); }
| arithmetic_expr CARET arithmetic_expr                    { $$ = make_arithmetic_power( std::move($1), std::move($3) ); }
| LEFT_ROUND_BRACE root_arithmetic_expr RIGHT_ROUND_BRACE  { $$ = std::move($2); }
;

root_arithmetic_expr:
  MINUS arithmetic_expr                               { $$ = make_arithmetic_negation( std::move($2) ); }
| arithmetic_expr                                     { $$ = std::move($1); }

root_rule:
  root_arithmetic_expr                                { root = $1; }

%%
