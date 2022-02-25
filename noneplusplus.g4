grammar noneplusplus;

primaryExpression
    : '(' expression ')'
    ;

expression
    : assignmentExpression (',' assignmentExpression)*
    ;

argumentExpressionList
    : primaryExpression (',' primaryExpression)* (',')?
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
    : '(' primaryExpression ')'
    | Identifier ('(' argumentExpressionList ')')?
    | Number
    | ('+'|'-') unaryExpression
    ;

expressionStatement
    : expression? (';' expression)* EndLine
    ;

statement
    : expressionStatement
    ;

declaration
    : statement
    ;

Identifier
    :  [a-zA-Z_]+
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