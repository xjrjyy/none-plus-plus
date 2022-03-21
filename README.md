# None++

解释器。

目前支持：

- 四则运算
- 部分数学函数
- 变量定义
- `if`
- `while`

```plain
a = 1, b = 2;
c = 3, d = 4;
print(+a);
print(a + -a);
a += (b += c * d);
print(cos(PI));
print(exp(1) - E);
print(ln(E));
if 1 { print(2); }
if 0 { print(1); }
a = 5;
while a { print(a); a -= 1; }
```

## TODO

- 重构代码
- 增加自定义函数功能
