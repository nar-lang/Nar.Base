export default function (runtime) {
    function cmpList(la, lb) {
        if (la.length < lb.length) {
            return -1;
        } else if (la.length > lb.length) {
            return 1;
        } else {
            for (let i = 0; i < la.length; i++) {
                const n = cmp(la[i], lb[i]);
                if (n !== 0) {
                    return n;
                }
            }
            return 0;
        }
    }

    function cmp(a, b) {
        if (a.kind !== b.kind) {
            throw "types are not equal";
        }
        switch (a.kind) {
            case runtime.INSTANCE_KIND_INT:
            case runtime.INSTANCE_KIND_FLOAT:
            case runtime.INSTANCE_KIND_CHAR:
            case runtime.INSTANCE_KIND_STRING:
                return (a.value < b.value) ? -1 : (a.value > b.value ? 1 : 0);
            case runtime.INSTANCE_KIND_LIST:
            case runtime.INSTANCE_KIND_TUPLE:
                const la = runtime.unwrapShallow(a);
                const lb = runtime.unwrapShallow(b);
                return cmpList(la, lb);
            case runtime.INSTANCE_KIND_UNIT:
                return 0;
            case runtime.INSTANCE_KIND_OPTION:
                if (a.name < b.name) {
                    return -1;
                } else if (a.name > b.name) {
                    return 1;
                } else {
                    if (a.name === "Nar.Core.Array.Array#ArrayImpl" && b.name === "Nar.Core.Array.Array#ArrayImpl") {
                        return cmp(
                            runtime.execute("Nar.Core.Array.toList", a),
                            runtime.execute("Nar.Core.Array.toList", b)
                        )
                    }
                    return cmpList(a.values, b.values);
                }
            case runtime.INSTANCE_KIND_RECORD:
                //TODO: slow
                const ra = runtime.unwrapShallow(a);
                const rb = runtime.unwrapShallow(b);
                const ka = Object.keys(ra);
                const kb = Object.keys(rb);
                if (ka.length < kb.length) {
                    return -1;
                } else if (ka.length > kb.length) {
                    return 1;
                } else {
                    ka.sort();
                    kb.sort();
                    if (ka.length < kb.length) {
                        return -1;
                    } else if (ka.length > kb.length) {
                        return 1;
                    } else {
                        for (let i = 0; i < ka.length; i++) {
                            if (ka[i] !== kb[i]) {
                                return ka[i] < kb ? -1 : 1;
                            }
                        }
                    }
                    for (let i = 0; i < ka.length; i++) {
                        const c = cmp(ra[ka[i]], rb[kb[i]]);
                        if (c !== 0) {
                            return c;
                        }
                    }
                    return 0;
                }

            case runtime.INSTANCE_KIND_FUNC:
                return (a.index < b.index) ? -1 : (a.index > b.index ? 1 : 0);
            case runtime.INSTANCE_KIND_NATIVE:
                if (a.value === b.value) {
                    return 0;
                } else if (a.value.length < b.value.length) {
                    return -1;
                } else {
                    return 1;
                }
            default:
                throw "enum case is out of range";
        }
    }

    function toString(x) {
        switch (x.kind) {
            case runtime.INSTANCE_KIND_CHAR:
                return `'${String.fromCodePoint(x.value).replaceAll("'", "\\\\'")}'`;
            case runtime.INSTANCE_KIND_STRING:
                return `"${x.value.replaceAll('"', '\\\\"')}"`;
            case runtime.INSTANCE_KIND_LIST:
                return `[${runtime.unwrapShallow(x).map(toString).join(", ")}]`;
            case runtime.INSTANCE_KIND_TUPLE:
                return `( ${runtime.unwrapShallow(x).map(toString).join(", ")} )`;
            case runtime.INSTANCE_KIND_RECORD:
                const r = runtime.unwrapShallow(x);
                return `{ ${Object.keys(r).map(k => k + " = " + toString(r[k])).join(", ")} }`;
            case runtime.INSTANCE_KIND_OPTION:
                return `${x.name}(${x.values.map(toString).join(", ")})`;
            case runtime.INSTANCE_KIND_NATIVE:
                return JSON.stringify(x.value);
            default:
                const s = runtime.unwrap(x);
                if (s === null) {
                    return "<null>";
                }
                return s.toString();
        }
    }

    runtime.scope("Nar.Core", {cmpList});

    runtime.register("Nar.Core.Basics", {
        eq: (a, b) => runtime.bool(cmp(a, b) === 0),
        neq: (a, b) => runtime.bool(cmp(a, b) !== 0),
        lt: (a, b) => runtime.bool(cmp(a, b) < 0),
        gt: (a, b) => runtime.bool(cmp(a, b) > 0),
        le: (a, b) => runtime.bool(cmp(a, b) <= 0),
        ge: (a, b) => runtime.bool(cmp(a, b) >= 0),
        not: (x) => runtime.bool(!runtime.unwrap(x)),
        and: (x, y) => runtime.bool(runtime.unwrap(x) && runtime.unwrap(y)),
        or: (x, y) => runtime.bool(runtime.unwrap(x) || runtime.unwrap(y)),
        xor: (x, y) => runtime.bool(runtime.unwrap(x) ^ runtime.unwrap(y)),
    });
    runtime.register("Nar.Core.Math", {
        add: (x, y) => {
            if (x.kind !== y.kind) {
                throw "types are not equal";
            }
            return Object.freeze({kind: x.kind, value: x.value + y.value});
        },
        sub: (x, y) => {
            if (x.kind !== y.kind) {
                throw "types are not equal";
            }
            return Object.freeze({kind: x.kind, value: x.value - y.value});
        },
        mul: (x, y) => {
            if (x.kind !== y.kind) {
                throw "types are not equal";
            }
            return Object.freeze({kind: x.kind, value: x.value * y.value})
        },
        div: (x, y) => {
            if (x.kind !== y.kind) {
                throw "types are not equal";
            }
            if (x.kind === runtime.INSTANCE_KIND_INT && y.kind === runtime.INSTANCE_KIND_INT) {
                if (y.value === 0) {
                    return runtime.int(NaN);
                }
                return runtime.int((x.value / y.value) | 0);
            } else {
                return runtime.float(x.value / y.value);
            }
        },
        neg: (x) => Object.freeze({kind: x.kind, value: -x.value}),
        abs: (x) => x.value >= 0 ? x : Object.freeze({kind: x.kind, value: -x.value}),
        toPower: (pow, num) => {
            if (pow.kind !== num.kind) {
                throw "types are not equal";
            }
            return Object.freeze({kind: num.kind, value: num.value ** pow.value});
        },
        isNan: (n) => runtime.bool(isNaN(n.value)),
        isInf: (n) => {
            const x = n.value;
            return runtime.bool(x === Infinity || x === -Infinity);
        },

        toFloat: (n) => runtime.float(n.value),
        round: (n) => runtime.int(Math.round(n.value)),
        floor: (n) => runtime.int(Math.floor(n.value)),
        ceil: (n) => runtime.int(Math.ceil(n.value)),
        trunc: (n) => runtime.int(Math.trunc(n.value)),
        sqrt: (n) => runtime.float(Math.sqrt(n.value)),
        remainderBy: (n, x) => runtime.int(x.value % n.value),
        modBy: (modulus, x) => {
            if (modulus.value === 0) {
                return runtime.int(NaN);
            }
            const answer = x.value % modulus.value;
            return runtime.int(((answer > 0 && modulus.value < 0) || (answer < 0 && modulus.value > 0)) ? answer + modulus.value : answer);
        },
        logBase: (base, n) => runtime.float(Math.log(n.value) / Math.log(base.value)),
    });
    runtime.register("Nar.Core.Bitwise", {
        and: (x, y) => runtime.int(x.value & y.value),
        or: (x, y) => runtime.int(x.value | y.value),
        xor: (x, y) => runtime.int(x.value ^ y.value),
        complement: (x) => runtime.int(~x.value),
        shiftLeftBy: (x, y) => runtime.int(y.value << x.value),
        shiftRightBy: (x, y) => runtime.int(y.value >> x.value),
        shiftRightZfBy: (x, y) => runtime.int(y.value >>> x.value),
    });
    runtime.register("Nar.Core.Char", {
        toUpper: (char) => runtime.char(String.fromCodePoint(char.value).toLocaleUpperCase().codePointAt(0)),
        toLower: (char) => runtime.char(String.fromCodePoint(char.value).toLocaleLowerCase().codePointAt(0)),
        toCode: (char) => runtime.int(char.value),
        fromCode: (code) => runtime.char(code.value),
    });
    runtime.register("Nar.Core.Debug", {
        toString: (x) => runtime.string(toString(x)),
        log: (msg, a) => {
            console.log(runtime.unwrap(msg) + toString(a));
            return a;
        },
        todo: (msg) => {
            console.error(runtime.unwrap(msg));
            console.debug();
            return runtime.unit();
        }
    });
    runtime.register("Nar.Core.List", {  //TODO: optimize all list functions
        cons: (head, tail) => runtime.listItem(head, tail),
        map2: (f, a, b) => {
            const la = runtime.unwrapShallow(a);
            const lb = runtime.unwrapShallow(b);
            const r = [];
            const n = Math.min(la.length, lb.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i]]));
            }
            return runtime.listShallow(r);
        },
        map3: (f, a, b, c) => {
            const la = runtime.unwrapShallow(a);
            const lb = runtime.unwrapShallow(b);
            const lc = runtime.unwrapShallow(c);
            const r = [];
            const n = Math.min(la.length, lb.length, lc.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i], lc[i]]));
            }
            return runtime.listShallow(r);
        },
        map4: (f, a, b, c, d) => {
            const la = runtime.unwrapShallow(a);
            const lb = runtime.unwrapShallow(b);
            const lc = runtime.unwrapShallow(c);
            const ld = runtime.unwrapShallow(d);
            const r = [];
            const n = Math.min(la.length, lb.length, lc.length, ld.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i], lc[i], ld[i]]));
            }
            return runtime.listShallow(r);
        },
        map5: (f, a, b, c, d, e) => {
            const la = runtime.unwrapShallow(a);
            const lb = runtime.unwrapShallow(b);
            const lc = runtime.unwrapShallow(c);
            const ld = runtime.unwrapShallow(d);
            const le = runtime.unwrapShallow(e);
            const r = [];
            const n = Math.min(la.length, lb.length, lc.length, ld.length, le.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i], lc[i], ld[i], le[i]]));
            }
            return runtime.listShallow(r);
        },
        sortWith: (f, xs) => {
            const l = runtime.unwrapShallow(xs);
            l.sort((a, b) => {
                const res = runtime.executeFn(f, [a, b]);
                if (res.name === "Nar.Core.Basics.Order#LT") {
                    return -1;
                }
                if (res.name === "Nar.Core.Basics.Order#GT") {
                    return 1;
                }
                if (res.name === "Nar.Core.Basics.Order#EQ") {
                    return 0;
                }
                throw "expected Nar.Core.Basics.Order";
            });
            return runtime.listShallow(l);
        },
        sortBy: (f, xs) => {
            const l = runtime.unwrapShallow(xs);
            l.sort((a, b) => {
                const xa = runtime.executeFn(f, [a]);
                const xb = runtime.executeFn(f, [b]);
                return cmp(xa, xb);
            });
            return runtime.listShallow(l);
        },
    });
    runtime.register("Nar.Core.String", {
        length: (s) => runtime.int(s.value.length),
        reverse: (s) => runtime.string([...runtime.unwrap(s)].reverse().join('')),
        append: (a, b) => runtime.string(runtime.unwrap(a) + runtime.unwrap(b)),
        split: (sep, string) => runtime.listShallow(runtime.unwrap(string).split(runtime.unwrap(sep)).map(runtime.string)),
        join: (sep, strings) => runtime.string(runtime.unwrap(strings).join(runtime.unwrap(sep))),
        words: (string) => runtime.listShallow(runtime.unwrap(string).trim().split(/\s+/).map(runtime.string)),
        lines: (string) => runtime.listShallow(runtime.unwrap(string).trim().split(/\r?\n/).map(runtime.string)),
        slice: (begin, end, s) => runtime.string(runtime.unwrap(s).slice(runtime.unwrap(begin), runtime.unwrap(end))),
        contains: (sub, string) => runtime.bool(runtime.unwrap(string).includes(runtime.unwrap(sub))),
        startsWith: (sub, string) => runtime.bool(runtime.unwrap(string).startsWith(runtime.unwrap(sub))),
        endsWith: (sub, string) => runtime.bool(runtime.unwrap(string).endsWith(runtime.unwrap(sub))),
        indices: (sub, string) => {
            const result = [];
            const s = runtime.unwrap(string);
            const u = runtime.unwrap(sub);

            if (u.length === 0) {
                return runtime.listShallow([]);
            }

            let idx = s.indexOf(u);

            while (idx !== -1) {
                result.push(idx);
                idx = s.indexOf(u, idx + 1);
            }
            return runtime.listShallow(result.map(runtime.int));
        },
        toUpper: (s) => runtime.string(runtime.unwrap(s).toLocaleUpperCase()),
        toLower: (s) => runtime.string(runtime.unwrap(s).toLocaleLowerCase()),
        trim: (s) => runtime.string(runtime.unwrap(s).trim()),
        trimLeft: (s) => runtime.string(runtime.unwrap(s).trimStart()),
        trimRight: (s) => runtime.string(runtime.unwrap(s).trimEnd()),
        toInt: (n) => {
            const maybe = runtime.qualifierIdentifier("Nar.Core.Maybe", "Maybe");
            const str = runtime.unwrap(n);
            const code0 = str.charCodeAt(0);
            const start = code0 === 0x2B /* + */ || code0 === 0x2D /* - */ ? 1 : 0;

            let i;
            let total = 0;
            for (i = start; i < str.length; ++i) {
                const code = str.charCodeAt(i);
                if (code < 0x30 || 0x39 < code) {
                    return runtime.optionShallow(maybe, "Nothing");
                }
                total = 10 * total + code - 0x30;
            }

            return i === start
                ? runtime.optionShallow(maybe, "Nothing")
                : runtime.optionShallow(maybe, "Just", [runtime.int(code0 === 0x2D ? -total : total)]);
        },
        fromInt: (n) => runtime.string(runtime.unwrap(n).toString()),
        toFloat: (n) => {
            const maybe = runtime.qualifierIdentifier("Nar.Core.Maybe", "Maybe");
            const s = runtime.unwrap(n);
            if (s.length === 0 || /[\sxbo]/.test(s)) {
                return runtime.optionShallow(maybe, "Nothing")
            }
            const x = +s;
            // faster isNaN check
            return x === x
                ? runtime.optionShallow(maybe, "Just", [runtime.float(x)])
                : runtime.optionShallow(maybe, "Nothing");
        },
        fromFloat: (n) => runtime.string(runtime.unwrap(n).toString()),
        fromList: (chars) => runtime.string(String.fromCodePoint(...runtime.unwrap(chars))),
        cons: (c, s) => runtime.string(String.fromCodePoint(runtime.unwrap(c)) + runtime.unwrap(s)),
        uncons: (str) => {
            const maybe = runtime.qualifierIdentifier("Nar.Core.Maybe", "Maybe");
            const s = runtime.unwrap(str);

            if (s.length === 0) {
                return runtime.optionShallow(maybe, "Nothing");
            } else {
                const c = s.codePointAt(0);
                return runtime.optionShallow(maybe, "Just", [runtime.tupleShallow([
                    runtime.char(c),
                    runtime.string(s.substring(c > 0xFFFF ? 2 : 1)),
                ])]);
            }
        },
        map: (f, str) => {
            return runtime.string(String.fromCodePoint(
                ...[...runtime.unwrap(str)].map(
                    c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.codePointAt(0))]))
                ))
            );
        },
        filter: (f, str) => {
            return runtime.string([...runtime.unwrap(str)].filter(
                    c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.codePointAt(0))]))
                ).join("")
            );
        },
        foldl: (f, acc, str) => {
            const s = [...runtime.unwrap(str)];
            for (let i = 0; i < s.length; i++) {
                acc = runtime.executeFn(f, [runtime.char(s[i].codePointAt(0)), acc]);
            }
            return acc;
        },
        foldr: (f, acc, str) => {
            const s = [...runtime.unwrap(str)];
            for (let i = s.length - 1; i >= 0; i--) {
                acc = runtime.executeFn(f, [runtime.char(s[i].codePointAt(0)), acc]);
            }
            return acc;
        },
        any: (f, str) => {
            return runtime.bool([...runtime.unwrap(str)].some(
                c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.codePointAt(0))]))
            ));
        },
        all: (f, str) => {
            return runtime.bool([...runtime.unwrap(str)].every(
                c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.codePointAt(0))]))
            ));
        },
    });
    runtime.register("Nar.Core.NativeArray", {
        empty: runtime.native([]),
        singleton: (item) => runtime.native([item]),
        length: (array) => {
            return runtime.int(array.value.length);
        },
        initialize: (size, offset, func) => {
            const result = new Array(size.value);
            for (let i = 0; i < size.value; i++) {
                result[i] = runtime.executeFn(func, [runtime.int(offset.value + i)]);
            }
            return runtime.native(result);
        },
        initializeFromList: (max, ls) => {
            const result = new Array(max.value);
            let i = 0;
            for (; i < max.value && ls && ls.value; i++) {
                result[i] = ls.value;
                ls = ls.next;
            }
            result.length = i;
            return runtime.tupleShallow([runtime.native(result), ls || runtime.listShallow([])]);
        },
        unsafeGet: (index, array) => {
            return array.value[index.value];
        },
        unsafeSet: (index, value, array) => {
            const length = array.value.length;
            const result = new Array(length);
            for (let i = 0; i < length; i++) {
                result[i] = array.value[i];
            }
            result[index.value] = value;
            return runtime.native(result);
        },
        push: (value, array) => {
            const length = array.value.length;
            const result = new Array(length + 1);
            for (let i = 0; i < length; i++) {
                result[i] = array.value[i];
            }
            result[length] = value;
            return runtime.native(result);
        },
        foldl: (func, acc, array) => {
            const length = array.value.length;
            for (let i = 0; i < length; i++) {
                acc = runtime.executeFn(func, [array.value[i], acc]);
            }
            return acc;
        },
        foldr: (func, acc, array) => {
            for (let i = array.value.length - 1; i >= 0; i--) {
                acc = runtime.executeFn(func, [array.value[i], acc]);
            }
            return acc;
        },
        map: (func, array) => {
            const length = array.value.length;
            const result = new Array(length);
            for (let i = 0; i < length; i++) {
                result[i] = runtime.executeFn(func, [array.value[i]]);
            }
            return runtime.native(result);
        },
        indexedMap: (func, offset, array) => {
            const length = array.value.length;
            const result = new Array(length);
            for (let i = 0; i < length; i++) {
                result[i] = runtime.executeFn(func, [runtime.int(offset.value + i), array.value[i]]);
            }
            return runtime.native(result);
        },
        slice: (from, to, array) => {
            return runtime.native(array.value.slice(from.value, to.value));
        },
        appendN: (n, dest, source) => {
            const destLen = dest.value.length;
            let itemsToCopy = n.value - destLen;
            if (itemsToCopy > source.value.length) {
                itemsToCopy = source.value.length;
            }
            const size = destLen + itemsToCopy;
            const result = new Array(size);
            for (let i = 0; i < destLen; i++) {
                result[i] = dest.value[i];
            }

            for (let i = 0; i < itemsToCopy; i++) {
                result[i + destLen] = source.value[i];
            }
            return runtime.native(result);
        },
    });
}
