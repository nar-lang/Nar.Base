package core

import "github.com/oaklang/runtime"

definedType List_List[T any] runtime.List[T]

func (xs List_List[T]) Head() T {
	return runtime.List[T](xs).Head()
}

func (xs List_List[T]) Tail() List_List[T] {
	return List_List[T](runtime.List[T](xs).Tail())
}

func (xs List_List[T]) IsEmpty() bool {
	return runtime.List[T](xs).IsEmpty()
}

//TODO: temporary, replace with list comparisions

func (xs List_List[T]) At(index int) T {
	return runtime.List[T](xs).At(index)
}

//TODO: temporary, replace with list comparisions

func (xs List_List[T]) Length() int {
	return runtime.List[T](xs).Length()
}

func List_cons[T any](T) func(List_List[T]) List_List[T] {
	return func(l List_List[T]) List_List[T] {
		panic("not implemented")
	}
}

func List_map2[a, b, result any](f func(a a) func(b b) result) func(la List_List[a]) func(lb List_List[b]) List_List[result] {
	return func(la List_List[a]) func(lb List_List[b]) List_List[result] {
		return func(lb List_List[b]) List_List[result] {
			panic("not implemented")
		}
	}
}

func List_map3[a, b, c, result any](f func(a a) func(b b) func(c c) result) func(la List_List[a]) func(lb List_List[b]) func(lc List_List[c]) List_List[result] {
	return func(la List_List[a]) func(lb List_List[b]) func(lc List_List[c]) List_List[result] {
		return func(lb List_List[b]) func(lc List_List[c]) List_List[result] {
			return func(lc List_List[c]) List_List[result] {
				panic("not implemented")
			}
		}
	}
}

func List_map4[a, b, c, d, result any](f func(a a) func(b b) func(c c) func(d d) result) func(la List_List[a]) func(lb List_List[b]) func(lc List_List[c]) func(ld List_List[d]) List_List[result] {
	return func(la List_List[a]) func(lb List_List[b]) func(lc List_List[c]) func(ld List_List[d]) List_List[result] {
		return func(lb List_List[b]) func(lc List_List[c]) func(ld List_List[d]) List_List[result] {
			return func(lc List_List[c]) func(ld List_List[d]) List_List[result] {
				return func(ld List_List[d]) List_List[result] {
					panic("not implemented")
				}
			}
		}
	}
}

func List_map5[a, b, c, d, e, result any](f func(a a) func(b b) func(c c) func(d d) func(e e) result) func(la List_List[a]) func(lb List_List[b]) func(lc List_List[c]) func(ld List_List[d]) func(le List_List[e]) List_List[result] {
	return func(la List_List[a]) func(lb List_List[b]) func(lc List_List[c]) func(ld List_List[d]) func(le List_List[e]) List_List[result] {
		return func(lb List_List[b]) func(lc List_List[c]) func(ld List_List[d]) func(le List_List[e]) List_List[result] {
			return func(lc List_List[c]) func(ld List_List[d]) func(le List_List[e]) List_List[result] {
				return func(ld List_List[d]) func(le List_List[e]) List_List[result] {
					return func(le List_List[e]) List_List[result] {
						panic("not implemented")
					}
				}
			}
		}
	}
}

func List_sortBy[a any, b runtime.Comparable](key func(a) b) func(xs List_List[a]) List_List[a] {
	return func(xs List_List[a]) List_List[a] {
		panic("not implemented")
	}
}

func List_sortWith[a any](order func(a a) func(a a) Basics_Order) func(xs List_List[a]) List_List[a] {
	return func(xs List_List[a]) List_List[a] {
		panic("not implemented")
	}
}
