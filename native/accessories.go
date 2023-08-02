package core

type oak_Union struct {
	Value any
	name  string
}

func oak_F2[P1, P2, R any](f func(P1, P2) R) func(P1) func(P2) R {
	return func(p1 P1) func(P2) R {
		return func(p2 P2) R {
			return f(p1, p2)
		}
	}
}
