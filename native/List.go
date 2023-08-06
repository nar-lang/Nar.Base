package core

import "github.com/oaklang/runtime"

type List_List[T any] runtime.List[T]

func List_cons[T any](T) func(List_List[T]) List_List[T] {
	return func(l List_List[T]) List_List[T] {
		panic("not implemented")
	}
}
