package core

import (
	"fmt"
	"log"
)

func Debug_toString[T any](x T) String_String {
	return String_String(fmt.Sprintf("%+v", x))
}

func Debug_log[T any](msg String_String) func(T) T {
	return func(x T) T {
		log.Printf("%s%s\n", string(msg), string(Debug_toString(x)))
		return x
	}
}

func Debug_todo[T any](msg String_String) T {
	log.Println(string(msg))
	var t T
	return t
}
