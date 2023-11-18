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
                const la = runtime.unwrap(a.value);
                const lb = runtime.unwrap(b.value);
                return cmpList(la, lb);
            case runtime.INSTANCE_KIND_UNIT:
                return 0;
            case runtime.INSTANCE_KIND_OPTION:
                if (a.name < b.name) {
                    return -1;
                } else if (a.name > b.name) {
                    return 1;
                } else {
                    return cmpList(a.values, b.values);
                }
            case runtime.INSTANCE_KIND_RECORD:
                if (a.key === undefined && b.key === undefined) {
                    return 0;
                } else if (a.key === undefined) {
                    return -1;
                } else if (b.key === undefined) {
                    return 1;
                } else {
                    if (a.key < b.key) {
                        return -1;
                    } else if (a.key > b.key) {
                        return 1;
                    } else {
                        const v = cmp(a.value, b.value);
                        if (v !== 0) {
                            return v;
                        }
                        return cmp(a.parent, b.parent);
                    }
                }
            case runtime.INSTANCE_KIND_FUNC:
                return (a.index < b.index) ? -1 : (a.index > b.index ? 1 : 0);
            default:
                throw "enum case is out of range";
        }
    }

    runtime.register("Oak.Core.Basics", {
        add: (x, y) => runtime.wrap(x.value + y.value),
        sub: (x, y) => runtime.wrap(x.value - y.value),
        mul: (x, y) => runtime.wrap(x.value * y.value),
        div: (x, y) => runtime.wrap(x.value / y.value),
        neg: (x) => runtime.wrap(-x.value),
        pow: (x, y) => runtime.wrap(Math.pow(Number(x.value), Number(y.value))),
        toFloat: (n) => runtime.float(Number(n.value)),
        round: (n) => runtime.int(BigInt(Math.round(n))),
        floor: (n) => runtime.int(BigInt(Math.floor(n))),
        ceil: (n) => runtime.int(BigInt(Math.trunc(n))),
        eq: (a, b) => runtime.bool(cmp(a, b) === 0),
        neq: (a, b) => runtime.bool(cmp(a, b) !== 0),
        lt: (a, b) => runtime.bool(cmp(a, b) < 0),
        gt: (a, b) => runtime.bool(cmp(a, b) > 0),
        le: (a, b) => runtime.bool(cmp(a, b) <= 0),
        ge: (a, b) => runtime.bool(cmp(a, b) >= 0),
        not: (x) => runtime.bool(!x.value),
        and: (x, y) => runtime.bool(runtime.unwrap(x) && runtime.unwrap(y)),
        or: (x, y) => runtime.bool(runtime.unwrap(x) || runtime.unwrap(y)),
        xor: (x, y) => runtime.bool(runtime.unwrap(x) ^ runtime.unwrap(y)),
    });
    runtime.register("Oak.Core.Bitwise", {
        and: (x, y) => runtime.int(x.value & y.value),
        or: (x, y) => runtime.int(x.value | y.value),
        xor: (x, y) => runtime.int(x.value ^ y.value),
        complement: (x) => runtime.int(~x.value),
        shiftLeftBy: (x, y) => runtime.int(y.value << x.value),
        shiftRightBy: (x, y) => runtime.int(y.value >> x.value),
    });
    runtime.register("Oak.Core.Char", {
        toUpper: (char) => runtime.char(String.fromCharCode(char.value).toLocaleUpperCase().charCodeAt(0)),
        toLower: (char) => runtime.char(String.fromCharCode(char.value).toLocaleLowerCase().charCodeAt(0)),
        toCode: (char) => runtime.int(BigInt(char.value)),
        fromCode: (code) => runtime.char(Number(code.value)),
    });
    runtime.register("Oak.Core.Debug", {
        toString: (x) => runtime.unwrap(x).toString(),
        log: (msg, a) => {
            console.log(runtime.unwrap(msg));
            return a;
        },
        todo: (msg) => {
            console.error(runtime.unwrap(msg));
            console.debug();
            return runtime.unit();
        }
    });
    runtime.register("Oak.Core.List", {  //TODO: optimize all list functions
        cons: (head, tail) => runtime._listItem(head, tail),
        map2: (f, a, b) => {
            const la = runtime.unwrap(a);
            const lb = runtime.unwrap(b);
            const r = [];
            const n = Math.min(la.length, lb.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i]]));
            }
            return runtime.list(r);
        },
        map3: (f, a, b, c) => {
            const la = runtime.unwrap(a);
            const lb = runtime.unwrap(b);
            const lc = runtime.unwrap(c);
            const r = [];
            const n = Math.min(la.length, lb.length, lc.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i], lc[i]]));
            }
            return runtime.list(r);
        },
        map4: (f, a, b, c, d) => {
            const la = runtime.unwrap(a);
            const lb = runtime.unwrap(b);
            const lc = runtime.unwrap(c);
            const ld = runtime.unwrap(d);
            const r = [];
            const n = Math.min(la.length, lb.length, lc.length, ld.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i], lc[i], ld[i]]));
            }
            return runtime.list(r);
        },
        map5: (f, a, b, c, d, e) => {
            const la = runtime.unwrap(a);
            const lb = runtime.unwrap(b);
            const lc = runtime.unwrap(c);
            const ld = runtime.unwrap(d);
            const le = runtime.unwrap(e);
            const r = [];
            const n = Math.min(la.length, lb.length, lc.length, ld.length, le.length);
            for (let i = 0; i < n; i++) {
                r.push(runtime.executeFn(f, [la[i], lb[i], lc[i], ld[i], le[i]]));
            }
            return runtime.list(r);
        },
        sortBy: (f, xs) => {
            const l = runtime.unwrap(xs);
            l.sort((a, b) => {
                const res = runtime.executeFn(f, [a, b]);
                if (res.name === "LT") {
                    return -1;
                }
                if (res.name === "GT") {
                    return 1;
                }
                return 0;
            });
            return runtime.list(l);
        },
        sortWith: (f, xs) => {
            const l = runtime.unwrap(xs);
            l.sort((a, b) => {
                const xa = runtime.executeFn(f, [a]);
                const xb = runtime.executeFn(f, [a]);
                if (lt(xa, xb).value) {
                    return -1;
                }
                if (gt(xa, xb).value) {
                    return 1;
                }
                return 0;
            });
            return runtime.list(l);
        },
    });
    runtime.register("Oak.Core.String", {
        length: (s) => s.value.length,
        reverse: (s) => runtime.string([...runtime.unwrap(s)].reverse().join('')),
        append: (a, b) => runtime.string(runtime.unwrap(a) + runtime.unwrap(b)),
        split: (sep, string) => runtime.list(runtime.unwrap(string).split(sep).map(runtime.string)),
        join: (sep, strings) => runtime.string(runtime.unwrap(strings).join(runtime.unwrap(sep))),
        words: (string) => runtime.list(runtime.unwrap(string).trim().split(/\s+/).map(runtime.string)),
        lines: (string) => runtime.list(runtime.unwrap(string).trim().split(/\r?\n/).map(runtime.string)),
        slice: (begin, end, s) => runtime.string(runtime.unwrap(s).slice(Number(runtime.unwrap(begin)), Number(runtime.unwrap(end)))),
        contains: (sub, string) => runtime.bool(runtime.unwrap(string).includes(runtime.unwrap(sub))),
        startsWith: (sub, string) => runtime.bool(runtime.unwrap(string).startsWith(runtime.unwrap(sub))),
        endsWith: (sub, string) => runtime.bool(runtime.unwrap(string).endsWith(runtime.unwrap(sub))),
        indices: (sub, string) => {
            const result = [];
            const s = runtime.unwrap(string);
            const u = runtime.unwrap(sub);
            let idx = s.indexOf(u);

            while (idx !== -1) {
                result.push(idx);
                idx = s.indexOf(u, idx + 1);
            }
            return runtime.list(result);
        },
        toUpper: (s) => runtime.string(runtime.unwrap(s).toLocaleUpperCase()),
        toLower: (s) => runtime.string(runtime.unwrap(s).toLocaleLowerCase()),
        trim: (s) => runtime.string(runtime.unwrap(s).trim()),
        trimLeft: (s) => runtime.string(runtime.unwrap(s).trimStart()),
        trimRight: (s) => runtime.string(runtime.unwrap(s).trimEnd()),
        toInt: (s) => {
            const maybe = runtime.qualifierIdentifier("Oak.Core.Maybe", "Maybe");
            try {
                return runtime.option(maybe, "Just", [runtime.int(BigInt(runtime.unwrap(s)))]);
            } catch {
                return runtime.option(maybe, "Nothing");
            }
        },
        fromInt: (n) => runtime.string(runtime.unwrap(n).toString()),
        toFloat: (s) => {
            const maybe = runtime.qualifierIdentifier("Oak.Core.Maybe", "Maybe");
            const f = parseFloat(runtime.unwrap(s));
            if (isNaN(f)) {
                return runtime.option(maybe, "Nothing");
            } else {
                return runtime.option(maybe, "Just", [runtime.float(f)]);
            }
        },
        fromFloat: (n) => runtime.string(runtime.unwrap(n).toString()),
        fromList: (chars) => runtime.string(String.fromCharCode(...runtime.unwrap(chars))),
        cons: (c, s) => runtime.string(String.fromCharCode(runtime.unwrap(c)) + runtime.unwrap(s)),
        uncons: (str) => {
            const maybe = runtime.qualifierIdentifier("Oak.Core.Maybe", "Maybe");
            const s = runtime.unwrap(str);
            if (s.length === 0) {
                return runtime.option(maybe, "Nothing");
            } else {
                return runtime.option(maybe, "Just", [runtime.tuple([
                    runtime.char(s.charCodeAt(0)),
                    runtime.string(s.substring(1)),
                ])]);
            }
        },
        map: (f, str) => {
            return runtime.string(String.fromCharCode(
                ...runtime.unwrap(str).split("").map(
                    c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.charCodeAt(0))]))
                ))
            );
        },
        filter: (f, str) => {
            return runtime.string(String.fromCharCode(
                ...runtime.unwrap(str).split("").filter(
                    c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.charCodeAt(0))]))
                ))
            );
        },
        foldl: (f, acc, str) => {
            const s = runtime.unwrap(str);
            for (let i = 0; i < s.length; i++) {
                acc = runtime.executeFn(f, [runtime.char(s.charCodeAt(i)), acc]);
            }
            return acc;
        },
        foldr: (f, acc, str) => {
            const s = runtime.unwrap(str);
            for (let i = s.length - 1; i >= 0; i--) {
                acc = runtime.executeFn(f, [runtime.char(s.charCodeAt(i)), acc]);
            }
            return acc;
        },
        any: (f, str) => {
            return runtime.string(String.fromCharCode(
                ...runtime.unwrap(str).split("").some(
                    c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.charCodeAt(0))]))
                ))
            );
        },
        all: (f, str) => {
            return runtime.string(String.fromCharCode(
                ...runtime.unwrap(str).split("").every(
                    c => runtime.unwrap(runtime.executeFn(f, [runtime.char(c.charCodeAt(0))]))
                ))
            );
        },
    })
}
