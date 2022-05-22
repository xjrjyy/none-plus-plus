#include "functions.h"

namespace Calc
{
FunctionType emptyFunction = [](const ArgsType&) -> NumberType {
	return NumberType();
};
}
