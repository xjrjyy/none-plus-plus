grammar noneplusplus;

primaryExpression
    : assignmentExpression
    ;

argumentExpressionList
    : primaryExpression (',' primaryExpression)* (',')?
    ;

assignmentExpression
    : additiveExpression ('=' primaryExpression)?
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

Identifier
    :  [a-zA-Z_]+
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