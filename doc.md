# Nar.Base — API Documentation


## Modules

- [Nar.Base.Array](#narbasearray)
- [Nar.Base.Basics](#narbasebasics)
- [Nar.Base.Bitwise](#narbasebitwise)
- [Nar.Base.Char](#narbasechar)
- [Nar.Base.Debug](#narbasedebug)
- [Nar.Base.Dict](#narbasedict)
- [Nar.Base.List](#narbaselist)
- [Nar.Base.Math](#narbasemath)
- [Nar.Base.Maybe](#narbasemaybe)
- [Nar.Base.NativeArray](#narbasenativearray)
- [Nar.Base.Queue](#narbasequeue)
- [Nar.Base.Result](#narbaseresult)
- [Nar.Base.Set](#narbaseset)
- [Nar.Base.String](#narbasestring)
- [Nar.Base.Tuple](#narbasetuple)

## Nar.Base.Array

### Types

#### `Array`

```
type Array[a]
```

Persistent random-access array, implemented as a bit-mapped trie
with a 32-wide branching factor. `get`/`set` are effectively O(1)
for typical sizes; `push` is amortised O(1).

### Definitions

#### `append`

```
append( (ArrayImpl(_, _, _, aTail) as a): Array[a], ArrayImpl(bLen, _, bTree, bTail): Array[a] ) -> Array[a]
```

Concatenate two arrays.

#### `bitMask`

```
bitMask: Int
```

Low-bit mask covering one branch-factor step (`0x1F` for the default
32-wide trie). Exposed mainly for the array implementation itself.

#### `empty`

```
empty: Array[a]
```

The empty array.

#### `filter`

```
filter(isGood: (a) -> Bool, array: Array[a]) -> Array[a]
```

Keep only the elements for which `isGood` returns `True`.

#### `foldl`

```
foldl(func: (a, b) -> b, baseCase: b, ArrayImpl(_, _, tree, tail): Array[a]) -> b
```

Left-associative fold (tail-recursive). Visits elements from the
first to the last.

#### `foldr`

```
foldr(func: (a, b) -> b, baseCase: b, ArrayImpl(_, _, tree, tail): Array[a]) -> b
```

Right-associative fold: visit elements from the last to the first.

#### `fromList`

```
fromList(list: List[a]) -> Array[a]
```

Build an array from a list, preserving order.

#### `get`

```
get(index: Int, ArrayImpl(len, startShift, tree, tail):Array[a]) -> Maybe[a]
```

Look up the element at `index`. Returns `Nothing` for out-of-range
indices (including negative).

#### `indexedMap`

```
indexedMap(func: (Int, a) -> b, ArrayImpl(len, _, tree, tail): Array[a]) -> Array[b]
```

Apply `func` to every element along with its index.

#### `initialize`

```
initialize(len: Int, fn: (Int) -> a) -> Array[a]
```

Build an array of length `len` where the element at index `i` is
`fn(i)`. Returns `empty` for `len <= 0`.

#### `isEmpty`

```
isEmpty(ArrayImpl(len, _, _, _): Array[a]) -> Bool
```

True when the array has no elements.

#### `length`

```
length(ArrayImpl(len, _, _, _): Array[a]) -> Int
```

Number of elements (O(1)).

#### `map`

```
map(func: (a) -> b, ArrayImpl(len, startShift, tree, tail): Array[a]) -> Array[b]
```

Apply `func` to every element, building a new array of the same
length.

#### `push`

```
push(a: a, (ArrayImpl(_, _, _, tail) as array): Array[a]) -> Array[a]
```

Append a single element to the end of the array.

#### `repeat`

```
repeat(n: Int, e: a) -> Array[a]
```

`repeat(n, e)` produces an array of length `n` filled with `e`.

#### `set`

```
set(index: Int, value: a, (ArrayImpl(len, startShift, tree, tail) as array): Array[a]) -> Array[a]
```

Replace the element at `index` with `value`. Out-of-range indices
leave the array unchanged.

#### `slice`

```
slice(from: Int, to: Int, array: Array[a]) -> Array[a]
```

Take a contiguous range of the array. `from` is inclusive, `to`
is exclusive; negative indices count from the end. Returns `empty`
when the range collapses.

#### `toIndexedList`

```
toIndexedList((ArrayImpl(len, _, _, _) as array): Array[a]) -> List[( Int, a )]
```

Like [`toList`](#tolist) but pairs each element with its index.

#### `toList`

```
toList(array: Array[a]) -> List[a]
```

Convert the array to a list, preserving order.

#### `unsafeReplaceTail`

```
unsafeReplaceTail(newTail: NativeArray[a], ArrayImpl(len, startShift, tree, tail): Array[a]) -> Array[a]
```

Replace the array's tail buffer with `newTail`. This is a low-level
primitive used by `push` and `append`; the caller is responsible
for keeping the array's invariants intact.

## Nar.Base.Basics

### Types

#### `Bool`

```
type Bool
  = True
  | False
```

The boolean type. Pattern-match on `True` / `False` or use the
dedicated operators `&&`, `||`, `^^`, and the `not` function.

#### `Never`

```
type Never
```

A type with no inhabitants. Useful as the error parameter of a
`Result` that can never fail, or as the success parameter of a
computation that runs forever.

#### `Order`

```
type Order
  = LT
  | EQ
  | GT
```

The result of a three-way comparison: `LT` (less), `EQ` (equal), or
`GT` (greater).

### Aliases

#### `Unit`

```
alias native Unit
```

The unit value, sole inhabitant of the `Unit` type. Returned by
functions that exist purely for their effects and have no useful
value to hand back.

### Definitions

#### `always`

```
always(a: a, _: b) -> a
```

Return the first argument, ignoring the second. Useful with `map`
to replace every element with a constant, or as `always(x)` to make
a constant function.

#### `and`

```
native and(x: Bool, y: Bool) -> Bool
```

Logical conjunction (short-circuiting via the `&&` operator).

#### `apL`

```
apL(f: (a) -> b, x: a) -> b
```

Backward pipe: apply a function to a value. `f <| x` is `f(x)`.

#### `apR`

```
apR(x: a, f: (a) -> b) -> b
```

Forward pipe: feed a value into a function. `x |> f` is `f(x)`.

#### `compare`

```
compare(l: cmp, r: cmp) -> Order
```

Three-way comparison reported as an `Order`. Handy when sorting or
implementing custom ordered structures.

#### `composeL`

```
composeL(g: (b) -> c, f: (a) -> b, x: a) -> c
```

Right-to-left function composition: `(g <~ f)(x) == g(f(x))`. Use
the `<~` operator.

#### `composeR`

```
composeR(f: (a) -> b, g: (b) -> c, x: a) -> c
```

Left-to-right function composition: `(f ~> g)(x) == g(f(x))`. Use
the `~>` operator.

#### `eq`

```
native eq(a: eq, b: eq) -> Bool
```

Structural equality. Works on any type that supports the `eq`
constraint (primitives, tuples, records, lists, custom types built
out of them). Use the `==` operator instead of calling this directly.

#### `ge`

```
native ge(l: cmp, r: cmp) -> Bool
```

Greater-than-or-equal. Use the `>=` operator.

#### `gt`

```
native gt(l: cmp, r: cmp) -> Bool
```

Strict greater-than. Use the `>` operator.

#### `identity`

```
identity(x: a) -> a
```

The identity function: returns its argument unchanged.

#### `le`

```
native le(l: cmp, r: cmp) -> Bool
```

Less-than-or-equal. Use the `<=` operator.

#### `lt`

```
native lt(l: cmp, r: cmp) -> Bool
```

Strict less-than. Works on any `cmp`-constrained type (numbers,
chars, strings, and tuples thereof). Use the `<` operator.

#### `max`

```
max(x: cmp, y: cmp) -> cmp
```

The larger of two comparable values. Ties return `x`.

#### `min`

```
min(x: cmp, y: cmp) -> cmp
```

The smaller of two comparable values. Ties return `x`.

#### `neq`

```
native neq(a: eq, b: eq) -> Bool
```

Structural inequality; the logical inverse of `eq`. Prefer the `!=`
operator at call sites.

#### `never`

```
never(x: Never) -> a
```

Eliminator for the empty `Never` type — since no value of type
`Never` can ever exist, this function can claim to produce any type.

#### `not`

```
native not(x: Bool) -> Bool
```

Logical negation.

#### `or`

```
native or(x: Bool, y: Bool) -> Bool
```

Logical disjunction (short-circuiting via the `||` operator).

#### `xor`

```
native xor(x: Bool, y: Bool) -> Bool
```

Logical exclusive-or.

### Operators

#### `!=`

```
infix (!=): (non 4) = neq
```

#### `&&`

```
infix (&&): (right 3) = and
```

#### `<`

```
infix (<): (non 4) = lt
```

#### `<=`

```
infix (<=): (non 4) = le
```

#### `<|`

```
infix (<|): (right 0) = apL
```

#### `<~`

```
infix (<~): (left 9) = composeL
```

#### `==`

```
infix (==): (non 4) = eq
```

#### `>`

```
infix (>): (non 4) = gt
```

#### `>=`

```
infix (>=): (non 4) = ge
```

#### `^^`

```
infix (^^): (right 2) = xor
```

#### `|>`

```
infix (|>): (left 0) = apR
```

#### `||`

```
infix (||): (right 2) = or
```

#### `~>`

```
infix (~>): (right 9) = composeR
```

## Nar.Base.Bitwise

### Definitions

#### `and`

```
native and(x: Int, y: Int) -> Int
```

Bitwise AND of two integers (use the `&` operator).

#### `complement`

```
native complement(x: Int) -> Int
```

Bitwise NOT (one's complement) of an integer.

#### `or`

```
native or(x: Int, y: Int) -> Int
```

Bitwise OR of two integers (use the `%` operator).

#### `shiftLeftBy`

```
native shiftLeftBy(x: Int, y: Int) -> Int
```

Shift `x` left by `y` bits, filling the low bits with zero.
The `<<` operator is `x << y == shiftLeftBy(y, x)`.

#### `shiftRightBy`

```
native shiftRightBy(x: Int, y: Int) -> Int
```

Arithmetic right shift: shift `x` right by `y` bits, preserving the
sign bit. The `>>` operator is `x >> y == shiftRightBy(y, x)`.

#### `shiftRightZfBy`

```
native shiftRightZfBy(x: Int, y: Int) -> Int
```

Logical right shift: shift `x` right by `y` bits, filling the high
bits with zero. The `>>>` operator is `x >>> y == shiftRightZfBy(y, x)`.

#### `xor`

```
native xor(x: Int, y: Int) -> Int
```

Bitwise exclusive-or of two integers (use the `^` operator).

### Operators

#### `%`

```
infix (%): (left 6) = or
```

#### `&`

```
infix (&): (left 7) = and
```

#### `<<`

```
infix (<<): (left 5) = shiftLeft
```

#### `>>`

```
infix (>>): (left 5) = shiftRight
```

#### `>>>`

```
infix (>>>): (left 5) = shiftRightZf
```

#### `^`

```
infix (^): (left 6) = xor
```

## Nar.Base.Char

### Aliases

#### `Char`

```
alias native Char
```

A single Unicode codepoint. Character literals use the `'x'` syntax.

### Definitions

#### `fromCode`

```
native fromCode(code: Int) -> Char
```

Build a character from a Unicode codepoint. Invalid codepoints
return the replacement character.

#### `isAlpha`

```
isAlpha(char: Char) -> Bool
```

True for any ASCII letter (a–z or A–Z).

#### `isAlphaNum`

```
isAlphaNum(char: Char) -> Bool
```

True for any ASCII letter or decimal digit.

#### `isDigit`

```
isDigit(char: Char) -> Bool
```

True for ASCII decimal digits 0–9.

#### `isHexDigit`

```
isHexDigit(char: Char) -> Bool
```

True for ASCII hexadecimal digits (0–9, a–f, A–F).

#### `isLower`

```
isLower (char: Char) -> Bool
```

True for ASCII lowercase letters a–z.

#### `isOctDigit`

```
isOctDigit(char: Char) -> Bool
```

True for ASCII octal digits 0–7.

#### `isUpper`

```
isUpper(char: Char) -> Bool
```

True for ASCII uppercase letters A–Z.

#### `toCode`

```
native toCode(char: Char) -> Int
```

Get the Unicode codepoint of a character as an integer.

#### `toLower`

```
native toLower(char: Char) -> Char
```

Convert a character to its lowercase equivalent (locale-aware).

#### `toUpper`

```
native toUpper(char: Char) -> Char
```

Convert a character to its uppercase equivalent (locale-aware).

## Nar.Base.Debug

### Types

#### `Type`

```
type Type
  = TypeUnknown
  | TypeUnit
  | TypeInt
  | TypeFloat
  | TypeString
  | TypeChar
  | TypeRecord
  | TypeTuple
  | TypeList
  | TypeOption
  | TypeFunction
  | TypeClosure
  | TypeNative
```

Runtime type tags reported by [`getType`](#gettype). The set is
closed: every Nar value falls into exactly one of these buckets.

### Definitions

#### `getType`

```
native getType(x: a) -> Type
```

Inspect the runtime tag of a value as a `Type` constructor. Useful
when writing generic native helpers.

#### `log`

```
native log(msg: String, a: a) -> a
```

Print `msg` along with `a` to the host log stream, then return `a`
unchanged so the call can be threaded through an expression.

#### `todo`

```
native todo(msg: String) -> a
```

Crash the program with the given message. Use as a placeholder
while developing code paths that aren't ready yet.

#### `toString`

```
native toString(x: a) -> String
```

Convert any value to a printable string, using the runtime's
built-in formatter. Useful for tracing and `log`.

## Nar.Base.Dict

### Types

#### `Dict`

```
type Dict[k, v]
```

Persistent ordered map from comparable keys (`cmp`) to values,
implemented as a left-leaning red-black tree. Lookups, insertions,
and deletions are O(log n).

### Definitions

#### `diff`

```
diff(t1: Dict[k,a], t2: Dict[k,b]) -> Dict[k,a]
```

Keep only the entries from the first dictionary whose key does NOT
appear in the second.

#### `empty`

```
empty: Dict[k,v]
```

The empty dictionary.

#### `filter`

```
filter(isGood:(k, v) -> Bool, dict: Dict[k,v]) -> Dict[k,v]
```

Keep only the entries for which `isGood` returns `True`.

#### `foldl`

```
foldl(func:(k, v, b) -> b, acc: b, dict: Dict[k,v]) -> b
```

Left-to-right fold over the dictionary in ascending key order.

#### `foldr`

```
foldr(func:(k, v, b) -> b, acc: b, t: Dict[k,v]) -> b
```

Right-to-left fold over the dictionary in ascending key order
(i.e. visits the smallest key last when threading right).

#### `fromList`

```
fromList(assocs: List[( cmp, v )]) -> Dict[cmp,v]
```

Build a dictionary from a list of pairs. Later entries with the
same key overwrite earlier ones.

#### `get`

```
get(targetKey: cmp, dict: Dict[cmp,v]) -> Maybe[v]
```

Look up the value bound to `targetKey`, or `Nothing` if the key is
absent.

#### `insert`

```
insert(key: cmp, value: v, dict: Dict[cmp,v]) -> Dict[cmp,v]
```

Insert or replace the binding for `key`.

#### `intersect`

```
intersect(t1: Dict[cmp,v], t2: Dict[cmp,v]) -> Dict[cmp,v]
```

Keep only the entries from the first dictionary whose key also
appears in the second.

#### `isEmpty`

```
isEmpty(dict: Dict[k,v]) -> Bool
```

True when the dictionary has no entries.

#### `keys`

```
keys(dict: Dict[k,v]) -> List[k]
```

All keys in ascending order.

#### `map`

```
map(func:(k,a) -> b, dict: Dict[k,b]) -> Dict[k,b]
```

Transform every value, passing the key alongside.

#### `member`

```
member(key: cmp, dict: Dict[cmp,v]) -> Bool
```

True when `key` is present in the dictionary.

#### `merge`

```
merge( leftStep:(k, a, r) -> r, bothStep:(k, a, b, r) -> r, rightStep:(k, b, r) -> r, leftDict: Dict[k,a], rightDict: Dict[k,b], initialResult: r ) -> r
```

Merge two dictionaries with key-aware callbacks. `leftStep` runs
for keys only in the left input, `rightStep` for keys only in the
right, and `bothStep` for keys present in both.

#### `partition`

```
partition(isGood:(cmp, v) -> Bool, dict:Dict[cmp,v]) -> ( Dict[cmp,v], Dict[cmp,v] )
```

Split a dictionary in two by a predicate.

#### `remove`

```
remove(key: k, dict: Dict[k,v]) -> Dict[k,v]
```

Remove the binding for `key`. If the key is absent the dictionary
is returned unchanged.

#### `singleton`

```
singleton(key: cmp, value: v) -> Dict[k,v]
```

Build a dictionary with exactly one entry.

#### `size`

```
size(dict: Dict[k,v]) -> Int
```

Number of key/value pairs in the dictionary.

#### `toList`

```
toList(dict: Dict[k,v]) -> List[( k, v )]
```

All (key, value) pairs in ascending key order.

#### `union`

```
union(t1: Dict[cmp,v], t2: Dict[cmp,v]) -> Dict[cmp,v]
```

Combine two dictionaries. On key collisions, values from the first
argument win.

#### `update`

```
update(targetKey: k, alter:(Maybe[v]) -> Maybe[v], dict: Dict[k,v]) -> Dict[k,v]
```

Update the binding at `targetKey` by passing the current value
(or `Nothing`) to `alter`. Returning `Nothing` from `alter` removes
the key; returning `Just(x)` inserts or replaces it.

#### `values`

```
values(dict: Dict[k,v]) -> List[v]
```

All values in ascending key order.

## Nar.Base.List

### Aliases

#### `List`

```
alias native List[a]
```

Immutable singly-linked list. Build with literal syntax
(`[1, 2, 3]`) or with the `cons` (`|`) operator.

### Definitions

#### `all`

```
all(isOkay: (a) -> Bool, list: List[a]) -> Bool
```

True if `isOkay` returns `True` for every element (vacuously true
for the empty list).

#### `any`

```
any(isOkay: (a) -> Bool, list: List[a]) -> Bool
```

True if `isOkay` returns `True` for at least one element.

#### `append`

```
append(xs: List[a], ys: List[a]) -> List[a]
```

Append two lists (use the `++` operator). Cost is O(length(xs)).

#### `concat`

```
concat(lists: List[List[a]]) -> List[a]
```

Flatten a list of lists into a single list.

#### `concatMap`

```
concatMap(f: (a) -> List[b], list: List[a]) -> List[b]
```

`map(f, xs) |> concat`: apply `f` to each element and concatenate
the resulting lists.

#### `cons`

```
native cons(head: a, tail: List[a]) -> List[a]
```

Prepend `head` to `tail`. Use the `|` operator at call sites.

#### `drop`

```
drop(n: Int, list: List[a]) -> List[a]
```

Drop the first `n` elements (or all of them if the list is shorter).

#### `filter`

```
filter(isGood: (a) -> Bool, list: List[a]) -> List[a]
```

Keep only the elements for which `isGood` returns `True`.

#### `filterMap`

```
filterMap(f: (a) -> Maybe[b], xs: List[a]) -> List[b]
```

Map and filter in one pass: `f` returns `Just(y)` to keep a mapped
value or `Nothing` to drop the element.

#### `foldl`

```
foldl(func: (a, b) -> b, acc: b, list: List[a]) -> b
```

Left-associative fold (tail-recursive). Processes the list head
first, accumulating into `acc`.

#### `foldr`

```
foldr(fn: (a, b) -> b, acc: b, ls: List[a]) -> b
```

Right-associative fold. Processes the list from the tail back, so
it builds a new list in the original order.

#### `head`

```
head(list: List[a]) -> Maybe[a]
```

First element, or `Nothing` for the empty list.

#### `indexedMap`

```
indexedMap(f: (Int, a) -> b, xs: List[a]) -> List[b]
```

Apply `f` to each element along with its zero-based index.

#### `intersperse`

```
intersperse(sep: a, xs: List[a]) -> List[a]
```

Insert `sep` between every pair of elements.

#### `isEmpty`

```
isEmpty(xs: List[a]) -> Bool
```

True if the list has no elements.

#### `length`

```
length(xs: List[a]) -> Int
```

Number of elements in the list (O(n)).

#### `map`

```
map(f: (a) -> b, xs: List[a]) -> List[b]
```

Apply `f` to every element of `xs`, preserving order.

#### `map2`

```
native map2(f: (a, b) -> r, a: List[a], b: List[b]) -> List[r]
```

Combine two lists element-wise with `f`. Length of the result is
the length of the shorter input.

#### `map3`

```
native map3(f: (a, b, c) -> r, a: List[a], b: List[b], c: List[c]) -> List[r]
```

Three-list variant of [`map2`](#map2).

#### `map4`

```
native map4(f: (a, b, c, d) -> r, a: List[a], b: List[b], c: List[c], d: List[d]) -> List[r]
```

Four-list variant of [`map2`](#map2).

#### `map5`

```
native map5(f: (a, b, c, d, e) -> r, a: List[a], b: List[b], c: List[c], d: List[d], e: List[e]) -> List[r]
```

Five-list variant of [`map2`](#map2).

#### `maximum`

```
maximum(list: List[cmp]) -> Maybe[cmp]
```

Largest element, or `Nothing` for an empty list.

#### `maybeCons`

```
maybeCons(f: (a) -> Maybe[b], mx: a, xs: List[b]) -> List[b]
```

#### `member`

```
member(x: eq, xs: List[eq]) -> Bool
```

True if `x` is in `xs`. Element type must support equality.

#### `minimum`

```
minimum(list: List[cmp]) -> Maybe[cmp]
```

Smallest element, or `Nothing` for an empty list.

#### `partition`

```
partition(pred: (a) -> Bool, list: List[a]) -> ( List[a], List[a] )
```

Split a list in two by a predicate. Elements satisfying `pred` go
on the left, the rest on the right; relative order is preserved
inside each bucket.

#### `product`

```
product(numbers: List[number]) -> number
```

Product of every element. One for an empty list.

#### `range`

```
range(lo: Int, hi: Int) -> List[Int]
```

Inclusive integer range: `range(1, 4) == [1, 2, 3, 4]`. Empty when
`hi < lo`.

#### `repeat`

```
repeat(n: Int, value: a) -> List[a]
```

`repeat(n, x)` produces an `n`-element list filled with `x`.

#### `reverse`

```
reverse(list: List[a]) -> List[a]
```

Reverse the order of the list (O(n)).

#### `singleton`

```
singleton(value: a) -> List[a]
```

Build a single-element list: `singleton(x) == [x]`.

#### `sort`

```
sort(xs: List[a]) -> List[a]
```

Sort by the natural order of the elements (`cmp` constraint).

#### `sortBy`

```
native sortBy(f: (a) -> b, xs: List[a]) -> List[a]
```

Sort by a key function. Elements are ordered by `f(x)`.

#### `sortWith`

```
native sortWith(f: (a, a) -> Order, xs: List[a]) -> List[a]
```

Sort with a custom comparator returning an `Order`.

#### `sum`

```
sum(numbers: List[number]) -> number
```

Sum of every element. Zero for an empty list.

#### `tail`

```
tail(list: List[a]) -> Maybe[List[a]]
```

Everything but the first element, or `Nothing` for the empty list.

#### `take`

```
take(n: Int, list: List[a]) -> List[a]
```

Take the first `n` elements (or fewer if the list is shorter).

#### `unzip`

```
unzip(pairs: List[( a, b )]) -> ( List[a], List[b] )
```

Inverse of `map2(pair, ...)`: split a list of pairs into two
parallel lists.

### Operators

#### `++`

```
infix (++): (right 5) = append
```

#### `|`

```
infix (|): (right 5) = cons
```

## Nar.Base.Math

### Aliases

#### `Float`

```
alias native Float
```

64-bit IEEE-754 floating point number.

#### `Int`

```
alias native Int
```

64-bit signed integer.

### Definitions

#### `abs`

```
native abs(x: number) -> number
```

Absolute value of a number.

#### `add`

```
native add(x: number, y: number) -> number
```

Numeric addition (the `+` operator). Works on `Int`–`Int` or
`Float`–`Float`; mixing requires an explicit `toFloat`.

#### `ceil`

```
native ceil(n: Float) -> Int
```

Smallest `Int` not less than `n`.

#### `div`

```
native div(x: number, y: number) -> number
```

Numeric division (the `/` operator). For `Int` this is truncating
integer division; for `Float` it's IEEE-754 division.

#### `e`

```
e: Float
```

Euler's number, the base of the natural logarithm (≈ 2.71828).

#### `floor`

```
native floor(n: Float) -> Int
```

Largest `Int` not greater than `n`.

#### `isInf`

```
native isInf(x: number) -> Bool
```

True if `x` is positive or negative infinity.

#### `isNan`

```
native isNan(x: number) -> Bool
```

True if `x` is the IEEE-754 not-a-number value.

#### `logBase`

```
native logBase(base: Float, n: Float) -> Float
```

Logarithm of `n` in the given `base`.

#### `modBy`

```
native modBy(n: Int, x: Int) -> Int
```

Integer modulus of `x` by `n`. Always non-negative when `n > 0`.

#### `mul`

```
native mul(x: number, y: number) -> number
```

Numeric multiplication (the `*` operator).

#### `neg`

```
native neg(x: number) -> number
```

Unary negation: `neg(x) == 0 - x`.

#### `pi`

```
pi: Float
```

The mathematical constant pi (≈ 3.14159).

#### `remainderBy`

```
native remainderBy(n: Int, x: Int) -> Int
```

Integer remainder of `x / n`. Result has the sign of `x`.

#### `round`

```
native round(n: Float) -> Int
```

Round a `Float` to the nearest `Int` (ties round to even).

#### `sqrt`

```
native sqrt(n: Float) -> Float
```

Principal (non-negative) square root.

#### `sub`

```
native sub(x: number, y: number) -> number
```

Numeric subtraction (the `-` operator).

#### `toFloat`

```
native toFloat(n: Int) -> Float
```

Convert an `Int` to its nearest `Float`.

#### `toPower`

```
native toPower(pow: number, n: number) -> number
```

`n` raised to the power `pow`.

#### `trunc`

```
native trunc(n: Float) -> Int
```

Truncate a `Float` toward zero, returning the integer part.

### Operators

#### `*`

```
infix (*): (left 7) = mul
```

#### `+`

```
infix (+): (left 6) = add
```

#### `-`

```
infix (-): (left 6) = sub
```

#### `/`

```
infix (/): (left 7) = div
```

## Nar.Base.Maybe

### Types

#### `Maybe`

```
type Maybe[a]
  = Just(a)
  | Nothing
```

Optional value: either `Just(x)` carrying a payload of type `a`,
or `Nothing` representing the absence of a value. Use this instead
of `null`/`nil` to make missing-value handling explicit in the
type system.

### Definitions

#### `andThen`

```
andThen(callback: (a) -> Maybe[b], maybeValue: Maybe[a]) -> Maybe[b]
```

Chain a computation that may itself fail. If `maybeValue` is
`Just(x)`, run `callback(x)`; otherwise propagate `Nothing`.

#### `destruct`

```
destruct(default: b, func: (a) -> b, maybe: Maybe[a]) -> b
```

Fold a `Maybe`: apply `func` to the inner value, or use `default`
if the value is missing. Like [`withDefault`](#withdefault) followed
by `map`, but in one step.

#### `isJust`

```
isJust(maybe: Maybe[a]) -> Bool
```

True if the `Maybe` is `Just _`.

#### `map`

```
map(f: (a) -> value, ma: Maybe[a]) -> Maybe[value]
```

Apply `f` to the inner value of a `Maybe`, propagating `Nothing`
unchanged.

#### `map2`

```
map2(f: (a,b) -> value, ma: Maybe[a], mb: Maybe[b]) -> Maybe[value]
```

Combine two `Maybe`s with a two-argument function. Returns
`Nothing` if either argument is `Nothing`.

#### `map3`

```
map3(f: (a,b,c) -> value, ma: Maybe[a], mb:Maybe[b], mc:Maybe[c]) -> Maybe[value]
```

Like [`map2`](#map2) but for three optional arguments.

#### `map4`

```
map4(f: (a,b,c,d) -> value, ma: Maybe[a], mb:Maybe[b], mc:Maybe[c], md:Maybe[d]) -> Maybe[value]
```

Like [`map2`](#map2) but for four optional arguments.

#### `map5`

```
map5(f: (a,b,c,d,e) -> value, ma: Maybe[a], mb:Maybe[b], mc:Maybe[c], md:Maybe[d], me:Maybe[e]) -> Maybe[value]
```

Like [`map2`](#map2) but for five optional arguments.

#### `withDefault`

```
withDefault(default: a, maybe: Maybe[a]) -> a
```

Unwrap a `Maybe`, returning `default` for `Nothing`.

## Nar.Base.NativeArray

### Aliases

#### `NativeArray`

```
alias native NativeArray[a]
```

Host-backed contiguous array. Used as a building block by
[`Array`](#nar-base-array) and other collection types. Most user
code should reach for `Array` or `List` instead — `NativeArray`
has no bounds checks on `unsafeGet`/`unsafeSet`.

### Definitions

#### `appendN`

```
native appendN(n: Int, from: NativeArray[a], to: NativeArray[a]) -> NativeArray[a]
```

Append up to `n` elements from `from` onto `to`, returning the
combined array.

#### `empty`

```
native empty: NativeArray[a]
```

Zero-length array constant.

#### `foldl`

```
native foldl(f: (a,b) -> b, acc: b, array: NativeArray[a]) -> b
```

Left-to-right fold.

#### `foldr`

```
native foldr(f: (a,b) -> b, acc: b, array: NativeArray[a]) -> b
```

Right-to-left fold.

#### `indexedMap`

```
native indexedMap(f: (Int, a) -> b, offset: Int, array: NativeArray[a]) -> NativeArray[b]
```

Apply `f` to every element along with its index (shifted by `offset`).

#### `initialize`

```
native initialize(n: Int, offset: Int, fn: (Int) -> a) -> NativeArray[a]
```

Build an `n`-element array where the value at index `i` is
`fn(i + offset)`.

#### `initializeFromList`

```
native initializeFromList(n: Int, ls: List[a]) -> ( NativeArray[a], List[a] )
```

Take up to `n` elements from `ls`, returning the array and the
(possibly empty) list of leftovers.

#### `length`

```
native length(arr: NativeArray[a]) -> Int
```

Number of elements stored in the array.

#### `map`

```
native map(f: (a) -> b, array: NativeArray[a]) -> NativeArray[b]
```

Apply `f` to every element.

#### `push`

```
native push(value: a, array: NativeArray[a]) -> NativeArray[a]
```

Return a new array with `value` appended.

#### `singleton`

```
native singleton(item: a) -> NativeArray[a]
```

One-element array containing `item`.

#### `slice`

```
native slice(start: Int, end: Int, array: NativeArray[a]) -> NativeArray[a]
```

Sub-array from `start` (inclusive) to `end` (exclusive). Negative
indices count from the end.

#### `unsafeGet`

```
native unsafeGet(index: Int, array: NativeArray[a]) -> a
```

Read the element at `index` with NO bounds check. Calling with an
out-of-range index is undefined behaviour.

#### `unsafeSet`

```
native unsafeSet(index: Int, value: a, array: NativeArray[a]) -> NativeArray[a]
```

Replace the element at `index` with NO bounds check. The original
array is unchanged.

## Nar.Base.Queue

### Types

#### `Queue`

```
type Queue[a]
  = QueueImpl(Front[a], Rear[a])
```

FIFO queue built from two lists: the front holds dequeue candidates
in order, the rear holds recently enqueued items in reverse. All
operations are amortized O(1).

### Definitions

#### `dequeue`

```
dequeue(QueueImpl(fl, rl): Queue[a]) -> ( Maybe[a], Queue[a] )
```

Remove the front element. Returns the popped value (or `Nothing` if
the queue was empty) paired with the queue after removal.

#### `empty`

```
empty: Queue[a]
```

The empty queue.

#### `enqueue`

```
enqueue(a: a, QueueImpl(fl, rl): Queue[a]) -> Queue[a]
```

Add an element to the back of the queue.

#### `filter`

```
filter(fc:(a) -> Bool, QueueImpl(fl, rl): Queue[a]) -> Queue[a]
```

Keep only the elements for which `fc` returns `True`.

#### `fromList`

```
fromList(list: List[a]) -> Queue[a]
```

Treat a list as a queue. The head of the list becomes the front.

#### `front`

```
front(QueueImpl(fl, _): Queue[a]) -> Maybe[a]
```

Peek at the front element without removing it.

#### `isEmpty`

```
isEmpty(QueueImpl(fl, rl): Queue[a]) -> Bool
```

True when the queue has no elements.

#### `map`

```
map(fc: (a) -> b, QueueImpl(fl, rl): Queue[a]) -> Queue[b]
```

Map a function over every element while preserving order.

#### `singleton`

```
singleton(a: a) -> Queue[a]
```

Build a queue holding exactly one element.

#### `size`

```
size(QueueImpl(fl, rl): Queue[a]) -> Int
```

Number of elements in the queue (O(n)).

#### `toList`

```
toList(QueueImpl(fl, rl): Queue[a]) -> List[a]
```

Flatten a queue back into a list in dequeue order.

#### `updateFront`

```
updateFront(f: (Maybe[a]) -> Maybe[a], QueueImpl(fl, rl): Queue[a]) -> Queue[a]
```

Apply `f` to the front of the queue. If `f` returns `Nothing` the
front element is removed; if `f` returns `Just(x)` the element is
replaced (or, for an empty queue, prepended).

## Nar.Base.Result

### Types

#### `Result`

```
type Result[error, value]
  = Ok(value)
  | Err(error)
```

A computation that either succeeded with a `value` or failed with
an `error`. Use this when failure carries useful information; for
plain presence/absence use [`Maybe`](#nar-base-maybe).

### Definitions

#### `andThen`

```
andThen(callback: (a) -> Result[x, b], result: Result[x,a]) -> Result[x, b]
```

Chain a computation that may also fail. If `result` is `Ok(x)`,
run `callback(x)`; otherwise propagate the existing `Err`.

#### `fromMaybe`

```
fromMaybe(err: x, maybe: Maybe[a]) -> Result[x, a]
```

Promote a `Maybe` to a `Result`, supplying a fixed error for the
`Nothing` case.

#### `isOk`

```
isOk(result: Result[x, a]) -> Bool
```

True if the result is `Ok _`.

#### `map`

```
map(func: (a) -> value, ra: Result[x, a]) -> Result[x, value]
```

Apply `func` to the success value, propagating `Err` unchanged.

#### `map2`

```
map2(func: (a,b) -> value, ra: Result[x, a], rb: Result[x, b]) -> Result[x, value]
```

Combine two results with a two-argument function. Returns the first
`Err` encountered if either input is an error.

#### `map3`

```
map3(func: (a,b,c) -> value, ra: Result[x, a], rb: Result[x, b], rc: Result[x, c]) -> Result[x, value]
```

Like [`map2`](#map2) but for three results.

#### `map4`

```
map4( func: (a,b,c,d) -> value, ra: Result[x, a], rb: Result[x, b], rc: Result[x, c], rd: Result[x, d] ) -> Result[x, value]
```

Like [`map2`](#map2) but for four results.

#### `map5`

```
map5( func: (a,b,c,d,e) -> value, ra: Result[x, a], rb: Result[x, b], rc: Result[x, c], rd: Result[x, d], re: Result[x, e] ) -> Result[x, value]
```

Like [`map2`](#map2) but for five results.

#### `mapError`

```
mapError(f: (x) -> y, result: Result[x, a]) -> Result[y, a]
```

Transform the error value, leaving `Ok` unchanged. Useful for
adapting an error type when crossing module boundaries.

#### `toMaybe`

```
toMaybe(result: Result[x, a]) -> Maybe[a]
```

Discard the error and produce a `Maybe`: `Ok(v) -> Just(v)`,
`Err(_) -> Nothing`.

#### `withDefault`

```
withDefault(default: a, result: Result[x, a]) -> a
```

Unwrap a `Result`, returning `default` for `Err _`.

## Nar.Base.Set

### Types

#### `Set`

```
type Set[t]
```

An unordered set of comparable values, implemented on top of
[`Dict`](#nar-base-dict). All operations require the element type
to satisfy the `cmp` constraint.

### Definitions

#### `diff`

```
diff((SetImpl(dict1)): Set[cmp], (SetImpl(dict2)): Set[cmp]) -> Set[cmp]
```

Set difference: elements in the first input that are not in the
second.

#### `empty`

```
empty: Set[a]
```

The empty set.

#### `filter`

```
filter(isGood: (cmp) -> Bool, (SetImpl(dict)): Set[cmp]) -> Set[cmp]
```

Keep only the elements for which `isGood` returns `True`.

#### `foldl`

```
foldl(func:(a,b) -> b, initialState: b, (SetImpl(dict)): Set[a]) -> b
```

Left-associative fold over the set in ascending order.

#### `foldr`

```
foldr(func:(a,b) -> b, initialState: b, (SetImpl(dict)): Set[a]) -> b
```

Right-associative fold over the set in ascending order.

#### `fromList`

```
fromList(list: List[cmp]) -> Set[cmp]
```

Build a set from a list, removing duplicates.

#### `insert`

```
insert(key: cmp, (SetImpl(dict)): Set[cmp]) -> Set[cmp]
```

Insert an element. If it was already present the set is returned
unchanged.

#### `intersect`

```
intersect((SetImpl(dict1)): Set[cmp], (SetImpl(dict2)): Set[cmp]) -> Set[cmp]
```

Set intersection: elements common to both inputs.

#### `isEmpty`

```
isEmpty(SetImpl(dict): Set[a]) -> Bool
```

True when the set has no elements.

#### `map`

```
map(func:(cmp) -> cmp2, set: Set[cmp]) -> Set[cmp2]
```

Apply `func` to every element. Note that the result type must also
be `cmp`, and duplicates in the mapped values collapse.

#### `member`

```
member(key: cmp, (SetImpl(dict)): Set[cmp]) -> Bool
```

True when `key` is a member of the set.

#### `partition`

```
partition(isGood:(cmp) -> Bool, (SetImpl(dict)): Set[cmp]) -> (Set[cmp], Set[cmp])
```

Split the set in two: elements satisfying `isGood` on the left, the
rest on the right.

#### `remove`

```
remove(key: cmp, (SetImpl(dict)): Set[cmp]) -> Set[cmp]
```

Remove an element. If it wasn't present the set is returned
unchanged.

#### `singleton`

```
singleton(key: cmp) -> Set[cmp]
```

A set containing exactly one element.

#### `size`

```
size((SetImpl(dict)): Set[a]) -> Int
```

Number of elements in the set.

#### `toList`

```
toList((SetImpl(dict)): Set[a]) -> List[a]
```

Convert a set to a list of its elements in ascending order.

#### `union`

```
union((SetImpl(dict1)): Set[cmp], (SetImpl(dict2)): Set[cmp]) -> Set[cmp]
```

Set union: every element that's in either input.

## Nar.Base.String

### Aliases

#### `String`

```
alias native String
```

Immutable Unicode string. Concatenate with the `<>` operator.

### Definitions

#### `all`

```
native all(f: (Char) -> Bool, s: String) -> Bool
```

True if `f` returns `True` for every character (vacuously true on
the empty string).

#### `any`

```
native any(f: (Char) -> Bool, s: String) -> Bool
```

True if `f` returns `True` for at least one character.

#### `append`

```
native append(a: String, b: String) -> String
```

Concatenate two strings (use the `<>` operator).

#### `concat`

```
concat(strings: List[String]) -> String
```

Concatenate a list of strings into one, with no separator.

#### `cons`

```
native cons(c: Char, s: String) -> String
```

Prepend a single character to a string.

#### `contains`

```
native contains(sub: String, string: String) -> Bool
```

True when `sub` appears anywhere in `string`.

#### `dropLeft`

```
dropLeft(n: Int, string: String) -> String
```

Drop the first `n` characters.

#### `dropRight`

```
dropRight(n: Int, string: String) -> String
```

Drop the last `n` characters.

#### `endsWith`

```
native endsWith(sub: String, string: String) -> Bool
```

True when `string` ends with `sub`.

#### `filter`

```
native filter(f: (Char) -> Bool, s: String) -> String
```

Keep only the characters for which `f` returns `True`.

#### `foldl`

```
native foldl(f: (Char,b) -> b, acc: b, s: String) -> b
```

Left-associative fold over the characters.

#### `foldr`

```
native foldr(f: (Char,b) -> b, acc: b, s: String) -> b
```

Right-associative fold over the characters.

#### `fromChar`

```
fromChar(char: Char) -> String
```

Build a one-character string from a `Char`.

#### `fromFloat`

```
native fromFloat(n: Float) -> String
```

Render a float as a decimal string.

#### `fromInt`

```
native fromInt(n: Int) -> String
```

Render an integer as a decimal string.

#### `fromList`

```
native fromList(chars: List[Char]) -> String
```

Assemble a string from a list of characters.

#### `indices`

```
native indices(sub: String, string: String) -> List[Int]
```

Every starting index at which `sub` occurs in `string`.

#### `isEmpty`

```
isEmpty(string: String) -> Bool
```

True for the zero-length string.

#### `join`

```
native join(sep: String, strings: List[String]) -> String
```

The inverse of [`split`](#split): join `strings` together, placing
`sep` between each pair.

#### `left`

```
left(n: Int, string: String) -> String
```

First `n` characters of the string. Empty for `n < 1`.

#### `length`

```
native length(s: String) -> Int
```

Number of `Char`s (Unicode codepoints) in the string.

#### `lines`

```
native lines(string: String) -> List[String]
```

Break the string into lines. Recognises `\n`, `\r\n`, and `\r`.

#### `map`

```
native map(f: (Char) -> Char, s: String) -> String
```

Map a function over every character.

#### `pad`

```
pad(n: Int, char: Char, string: String) -> String
```

Pad `string` symmetrically with `char` until it is `n` characters
long. If the gap is odd, the extra character goes to the left.

#### `padLeft`

```
padLeft(n: Int, char: Char, string: String) -> String
```

Pad on the left with `char` until the string is `n` characters long.

#### `padRight`

```
padRight(n: Int, char: Char, string: String) -> String
```

Pad on the right with `char` until the string is `n` characters long.

#### `repeat`

```
repeat(n: Int, chunk: String) -> String
```

`repeat(3, "ab")` produces `"ababab"`. Returns `""` for `n <= 0`.

#### `replace`

```
replace(before: String, after: String, string: String) -> String
```

Replace every occurrence of `before` with `after` in `string`.

#### `reverse`

```
native reverse(s: String) -> String
```

Reverse the order of characters.

#### `right`

```
right(n: Int, string: String) -> String
```

Last `n` characters of the string. Empty for `n < 1`.

#### `slice`

```
native slice(begin: Int, end: Int, s: String) -> String
```

Substring from byte/char offset `begin` (inclusive) to `end`
(exclusive). Negative indices count from the end.

#### `split`

```
native split(sep: String, string: String) -> List[String]
```

Split a string on every occurrence of `sep`. Empty `sep` is
undefined behaviour.

#### `startsWith`

```
native startsWith(sub: String, string: String) -> Bool
```

True when `string` starts with `sub`.

#### `toFloat`

```
native toFloat(s: String) -> Maybe[Float]
```

Parse a decimal floating-point literal. Returns `Nothing` on
failure.

#### `toInt`

```
native toInt(s: String) -> Maybe[Int]
```

Parse a decimal integer. Returns `Nothing` if the string is not a
valid integer literal.

#### `toList`

```
toList(string: String) -> List[Char]
```

Decompose the string into a list of its characters.

#### `toLower`

```
native toLower(s: String) -> String
```

Convert the entire string to lowercase (locale-aware).

#### `toUpper`

```
native toUpper(s: String) -> String
```

Convert the entire string to uppercase (locale-aware).

#### `trim`

```
native trim(s: String) -> String
```

Drop whitespace from both ends.

#### `trimLeft`

```
native trimLeft(s: String) -> String
```

Drop whitespace from the left end only.

#### `trimRight`

```
native trimRight(s: String) -> String
```

Drop whitespace from the right end only.

#### `uncons`

```
native uncons(s: String) -> Maybe[( Char, String )]
```

Split off the first character. Returns `Nothing` for the empty
string, otherwise `Just((head, tail))`.

#### `words`

```
native words(string: String) -> List[String]
```

Break the string into whitespace-separated words. Runs of
whitespace are collapsed and leading/trailing whitespace dropped.

### Operators

#### `<>`

```
infix (<>): (right 5) = append
```

## Nar.Base.Tuple

### Definitions

#### `first`

```
first(( x, _ ): ( x, y )) -> x
```

Project the first element of a 2-tuple.

#### `mapBoth`

```
mapBoth(funcA: (x) -> a, funcB: (y) -> b, ( x, y ): ( x, y )) -> ( a, b )
```

Transform both elements of a 2-tuple with independent functions.

#### `mapFirst`

```
mapFirst(func: (x) -> a, ( x, y ): ( x, y )) -> ( a, y )
```

Transform the first element of a 2-tuple, leaving the second alone.

#### `mapSecond`

```
mapSecond(func: (y) -> b, ( x, y ): ( x, y )) -> ( x, b )
```

Transform the second element of a 2-tuple, leaving the first alone.

#### `pair`

```
pair(x: x, y: y) -> ( x, y )
```

Build a 2-tuple from its two components. Equivalent to writing
`( x, y )` directly; provided for use in pipelines.

#### `second`

```
second(( _, y ): ( x, y )) -> y
```

Project the second element of a 2-tuple.
