package core

import (
	"github.com/oaklang/runtime"
	"math"
)

type Basics_Int = runtime.Int
type Basics_Float = runtime.Float

const IntBitSize = 64
const FloatBitSize = 64

func Basics_add[num runtime.Number](a num) func(num) num {
	return func(b num) num {
		return a + b
	}
}

func Basics_sub[num runtime.Number](a num) func(num) num {
	return func(b num) num {
		return a - b
	}
}

func Basics_mul[num runtime.Number](a num) func(num) num {
	return func(b num) num {
		return a * b
	}
}

var Basics_fdiv = runtime.F2(func(a, b Basics_Float) Basics_Float {
	return a / b
})

var Basics_idiv = runtime.F2(func(a, b Basics_Int) Basics_Int {
	return a / b
})

func Basics_pow[num runtime.Number](a num) func(num) num {
	return func(b num) num {
		return num(math.Pow(float64(a), float64(b)))
	}
}

func Basics_neg[num runtime.Number](a num) num {
	return -a
}

func Basics_toFloat(x Basics_Int) Basics_Float {
	return Basics_Float(x)
}

func Basics_round(x Basics_Float) Basics_Int {
	return Basics_Int(math.Round(float64(x)))
}

func Basics_floor(x Basics_Float) Basics_Int {
	return Basics_Int(math.Floor(float64(x)))
}

func Basics_ceiling(x Basics_Float) Basics_Int {
	return Basics_Int(math.Ceil(float64(x)))
}

func Basics_truncate(x Basics_Float) Basics_Int {
	return Basics_Int(math.Trunc(float64(x)))
}

func Basics_eq[c runtime.Equatable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if runtime.AreEqual(a, b) {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_neq[c runtime.Equatable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if runtime.AreEqual(a, b) {
			return Basics_False()
		}
		return Basics_True()
	}
}

func Basics_lt[c runtime.Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a < b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_gt[c runtime.Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a > b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_le[c runtime.Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a <= b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_ge[c runtime.Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a >= b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_compare[c runtime.Comparable](a c) func(b c) Basics_Order {
	return func(b c) Basics_Order {
		if a < b {
			return Basics_LT()
		} else if a > b {
			return Basics_GT()
		}
		return Basics_EQ()
	}
}

func Basics_not(x Basics_Bool) Basics_Bool {
	return ToBool(!IsTrue(x))
}

var Basics_and = runtime.F2(func(x, y Basics_Bool) Basics_Bool {
	return ToBool(IsTrue(x) && IsTrue(y))
})

var Basics_or = runtime.F2(func(x, y Basics_Bool) Basics_Bool {
	return ToBool(IsTrue(x) || IsTrue(y))
})

var Basics_xor = runtime.F2(func(x, y Basics_Bool) Basics_Bool {
	return ToBool(IsTrue(x) != IsTrue(y))
})

func IsTrue(x Basics_Bool) bool {
	return x.Value == "True"
}

func ToBool(x bool) Basics_Bool {
	if x {
		return Basics_True()
	}
	return Basics_False()
}
