grammar noneplusplus;

primaryExpression
    : '(' expression ')'
    ;

expression
    : assignmentExpression (',' assignmentExpression)*
    ;

argumentExpressionList
    : assignmentExpression (',' assignmentExpression)* (',')?
    ;

assignmentExpression
    : additiveExpression (('='|'+='|'-='|'*='|'/=') assignmentExpression)?
    ;

additiveExpression
    : multiplicativeExpression (('+'|'-') multiplicativeExpression)*
    ;

multiplicativeExpression
    : unaryExpression (('*'|'/') unaryExpression)*
    ;

unaryExpression
    : Identifier ('(' argumentExpressionList ')')?
    | Number
    | ('+'|'-') unaryExpression
    | primaryExpression
    ;

expressionStatement
    : expression? (';' expression)* EndLine
    ;

statement
    : expressionStatement
    ;

functionDefinition
    : Definition Identifier 
    '(' (Identifier (',' Identifier)*)? ')' 
    '{' statement* '}'
    ;

declaration
    : functionDefinition
    | statement
    ;

Identifier
    :  [a-zA-Z_]+
    ;

Definition
    : 'def'
    ;

fragment
EndLine
    : '\n'
    ;

fragment
DecimalNumber
    : ([1-9][0-9]*)
    | ('0')
    ;

fragment
Number
    :
        '0' (
            'b' [01]+
            | 'x' ([0-9a-fA-F])+
            | [0-7]+
        )
    |
        DecimalNumber
        ('.' [0-9]*)?
        (
            [eE][+-]?
            DecimalNumber
        )?
    ;