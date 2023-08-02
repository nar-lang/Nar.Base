package core

var Bitwise_and = oak_F2(func(x, y Basics_Int) Basics_Int {
	return x & y
})

var Bitwise_or = oak_F2(func(x, y Basics_Int) Basics_Int {
	return x | y
})

var Bitwise_xor = oak_F2(func(x, y Basics_Int) Basics_Int {
	return x ^ y
})

func Bitwise_complement(x Basics_Int) Basics_Int {
	return ^x
}

var Bitwise_shiftLeftBy = oak_F2(func(x, y Basics_Int) Basics_Int {
	return x << y
})

func Bitwise_shiftRightBy(x, y Basics_Int) Basics_Int {
	return x >> y
}
