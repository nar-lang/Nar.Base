package core

import (
	"unicode"
)

definedType Char_Char = rune

func Char_toUpper(char Char_Char) Char_Char {
	return unicode.ToUpper(char)
}

func Char_toLower(char Char_Char) Char_Char {
	return unicode.ToLower(char)
}

func Char_toCode(char Char_Char) Basics_Int {
	return Basics_Int(char)
}

func Char_fromCode(v Basics_Int) Char_Char {
	return Char_Char(v)
}
