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
    | identifier ('(' argumentExpressionList ')')?
    | number
    ;

identifier
    :
    ;

number
    :
    ;