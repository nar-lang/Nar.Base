package core

import (
	"github.com/oaklang/runtime"
	"strconv"
	"strings"
	"unicode"
)

definedType String_String string

func String_length(s String_String) Basics_Int {
	return Basics_Int(len(s))
}

func String_reverse(s String_String) String_String {
	r := []rune(s)
	for i, j := 0, len(r)-1; i < len(r)/2; i, j = i+1, j-1 {
		r[i], r[j] = r[j], r[i]
	}
	return String_String(r)
}

var String_append = runtime.F2(func(a String_String, b String_String) String_String {
	return a + b
})

var String_split = runtime.F2(func(sep String_String, s String_String) List_List[String_String] {
	return List_List[String_String](runtime.SliceToListCast[string, String_String](strings.Split(string(s), string(sep))))
})

var String_join = runtime.F2(func(sep String_String, s List_List[String_String]) String_String {
	return String_String(strings.Join(runtime.ListToSliceCast[String_String, string](runtime.List[String_String](s)), string(sep)))
})

func String_words(s String_String) List_List[String_String] {
	return List_List[String_String](runtime.SliceToListCast[string, String_String](strings.Fields(string(s))))
}

func String_lines(s String_String) List_List[String_String] {
	return List_List[String_String](runtime.SliceToListCast[string, String_String](strings.FieldsFunc(
		string(s),
		func(c rune) bool { return c == '\n' || c == '\r' }),
	))
}

var String_slice = runtime.F3(func(start Basics_Int, end Basics_Int, s String_String) String_String {
	if start < 0 {
		start += Basics_Int(len(s))
	}
	if end < 0 {
		end += Basics_Int(len(s))
	}
	return String_String(string(s)[start:end])
})

var String_contains = runtime.F2(func(sub String_String, s String_String) Basics_Bool {
	return ToBool(strings.Contains(string(s), string(sub)))
})

var String_startsWith = runtime.F2(func(sub String_String, s String_String) Basics_Bool {
	return ToBool(strings.HasPrefix(string(s), string(sub)))
})

var String_endsWith = runtime.F2(func(sub String_String, s String_String) Basics_Bool {
	return ToBool(strings.HasSuffix(string(s), string(sub)))
})

var String_indices = runtime.F2(func(sub String_String, s String_String) List_List[Basics_Int] {
	var indices []Basics_Int
	it := string(s)
	offset := 0
	for {
		n := strings.Index(it, string(sub))
		if n < 0 {
			break
		}
		indices = append(indices, Basics_Int(offset+n))
		it = it[n+1:]
		offset += n + 1
	}
	return List_List[Basics_Int](runtime.SliceToList(indices))
})

func String_toUpper(s String_String) String_String {
	return String_String(strings.ToUpper(string(s)))
}

func String_toLower(s String_String) String_String {
	return String_String(strings.ToLower(string(s)))
}

func String_trim(s String_String) String_String {
	return String_String(strings.TrimSpace(string(s)))
}

func String_trimLeft(s String_String) String_String {
	return String_String(strings.TrimLeftFunc(string(s), unicode.IsSpace))
}

func String_trimRight(s String_String) String_String {
	return String_String(strings.TrimRightFunc(string(s), unicode.IsSpace))
}

func String_toInt(s String_String) Maybe_Maybe[Basics_Int] {
	if i, err := strconv.ParseInt(string(s), 10, IntBitSize); err == nil {
		return Maybe_Just[Basics_Int](Basics_Int(i))
	} else {
		return Maybe_Nothing[Basics_Int]()
	}
}

func String_fromInt(x Basics_Int) String_String {
	return String_String(strconv.FormatInt(int64(x), 10))
}

func String_toFloat(s String_String) Maybe_Maybe[Basics_Float] {
	if f, err := strconv.ParseFloat(string(s), FloatBitSize); err == nil {
		return Maybe_Just[Basics_Float](Basics_Float(f))
	} else {
		return Maybe_Nothing[Basics_Float]()
	}
}

func String_fromFloat(x Basics_Float) String_String {
	return String_String(strconv.FormatFloat(float64(x), 'g', -1, FloatBitSize))
}

func String_fromList(list List_List[Char_Char]) String_String {
	return String_String(runtime.ListToSliceCast[Char_Char, rune](runtime.List[Char_Char](list)))
}

var String_cons = runtime.F2(func(c Char_Char, s String_String) String_String {
	return String_String(append([]rune{c}, []rune(s)...))
})

func String_uncons(s String_String) Maybe_Maybe[runtime.TypeTuple2[Char_Char, String_String]] {
	if len(s) > 0 {
		return Maybe_Just(runtime.TypeTuple2[Char_Char, String_String]{
			P0: Char_Char(s[0]),
			P1: s[1:],
		})
	} else {
		return Maybe_Nothing[runtime.TypeTuple2[Char_Char, String_String]]()
	}
}

var String_map = runtime.F2(func(fn func(Char_Char) Char_Char, s String_String) String_String {
	return String_String(strings.Map(fn, string(s)))
})

var String_filter = runtime.F2(func(fn func(Char_Char) Basics_Bool, s String_String) String_String {
	res := make([]rune, 0, len(s))

	for _, r := range s {
		if IsTrue(fn(r)) {
			res = append(res, r)
		}
	}

	return String_String(res)
})

func String_foldl[T any](fn func(Char_Char) func(T) T) func(T) func(String_String) T {
	return func(acc T) func(String_String) T {
		return func(s String_String) T {
			for _, r := range s {
				acc = fn(r)(acc)
			}

			return acc
		}
	}
}

func String_foldr[T any](fn func(Char_Char) func(T) T) func(T) func(String_String) T {
	return func(acc T) func(String_String) T {
		return func(s String_String) T {
			for i := len(s) - 1; i >= 0; i-- {
				acc = fn(Char_Char(s[i]))(acc)
			}
			return acc
		}
	}
}

var String_any = runtime.F2(func(fn func(Char_Char) Basics_Bool, s String_String) Basics_Bool {
	for _, r := range s {
		if IsTrue(fn(r)) {
			return Basics_True()
		}
	}
	return Basics_False()
})

var String_all = runtime.F2(func(fn func(Char_Char) Basics_Bool, s String_String) Basics_Bool {
	for _, r := range s {
		if !IsTrue(fn(r)) {
			return Basics_False()
		}
	}
	return Basics_True()
})
