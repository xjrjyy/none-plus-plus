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
    : numberExpression (('*'|'/') numberExpression)*
    ;

numberExpression
    : '(' primaryExpression ')'
    | Identifier ('(' argumentExpressionList ')')?
    | Number
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