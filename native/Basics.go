package core

import (
	"golang.org/x/exp/constraints"
	"math"
)

type Basics_Int = int64
type Basics_Float = float64
type Number = interface{ Basics_Int | Basics_Float }
type Comparable = constraints.Ordered
type Equatable = comparable

func Basics_add[num Number](a num) func(num) num {
	return func(b num) num {
		return a + b
	}
}

func Basics_sub[num Number](a num) func(num) num {
	return func(b num) num {
		return a - b
	}
}

func Basics_mul[num Number](a num) func(num) num {
	return func(b num) num {
		return a * b
	}
}

var Basics_fdiv = oak_F2(func(a, b Basics_Float) Basics_Float {
	return a / b
})

var Basics_idiv = oak_F2(func(a, b Basics_Int) Basics_Int {
	return a / b
})

func Basics_pow[num Number](a num) func(num) num {
	return func(b num) num {
		return num(math.Pow(float64(a), float64(b)))
	}
}

func Basics_toFloat(x Basics_Int) Basics_Float {
	return Basics_Float(x)
}

func Basics_round(x Basics_Float) Basics_Int {
	return Basics_Int(math.Round(x))
}

func Basics_floor(x Basics_Float) Basics_Int {
	return Basics_Int(math.Floor(x))
}

func Basics_ceiling(x Basics_Float) Basics_Int {
	return Basics_Int(math.Ceil(x))
}

func Basics_truncate(x Basics_Float) Basics_Int {
	return Basics_Int(math.Trunc(x))
}

func Basics_eq[c Equatable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a == b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_neq[c Equatable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a != b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_lt[c Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a < b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_gt[c Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a > b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_le[c Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a <= b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_ge[c Comparable](a c) func(b c) Basics_Bool {
	return func(b c) Basics_Bool {
		if a >= b {
			return Basics_True()
		}
		return Basics_False()
	}
}

func Basics_compare[c Comparable](a c) func(b c) Basics_Order {
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

var Basics_and = oak_F2(func(x, y Basics_Bool) Basics_Bool {
	return ToBool(IsTrue(x) && IsTrue(y))
})

var Basics_or = oak_F2(func(x, y Basics_Bool) Basics_Bool {
	return ToBool(IsTrue(x) || IsTrue(y))
})

var Basics_xor = oak_F2(func(x, y Basics_Bool) Basics_Bool {
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
