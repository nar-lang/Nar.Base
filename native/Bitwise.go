package core

import "github.com/oaklang/runtime"

var Bitwise_and = runtime.F2(func(x, y Basics_Int) Basics_Int {
	return x & y
})

var Bitwise_or = runtime.F2(func(x, y Basics_Int) Basics_Int {
	return x | y
})

var Bitwise_xor = runtime.F2(func(x, y Basics_Int) Basics_Int {
	return x ^ y
})

func Bitwise_complement(x Basics_Int) Basics_Int {
	return ^x
}

var Bitwise_shiftLeftBy = runtime.F2(func(x, y Basics_Int) Basics_Int {
	return x << y
})

var Bitwise_shiftRightBy = runtime.F2(func(x, y Basics_Int) Basics_Int {
	return x >> y
})
