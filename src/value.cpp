#include "value.h"

Value::Value(const std::string &name, const NumberType &value)
    : name(name), value(value) {}
Value::Value(const NumberType &value)
    : name("@unknown"), value(value) {}
std::string Value::GetName() const { return name; }
Value::NumberType &Value::GetValue() { return value; }
Value::NumberType Value::GetValue() const { return value; }
