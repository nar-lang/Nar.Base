--- Nar.Base native implementations (Lua), ported from JavaScript.
---
--- The package returns `function(rt)` so the host CLI can wire natives into a
--- specific `Runtime` instance after loading the bytecode. Helpers that
--- reference `rt` (e.g. `cmp`) live inside the returned function so they
--- close over the real runtime, not the module table.
local _Runtime = require("lunar.runtime")
local Object = _Runtime.Object
local ObjectKind = _Runtime.ObjectKind

return function(rt)
    -- Helper: compare two values according to Nar semantics
    local cmp -- forward declaration (mutually recursive with cmpList)

    local function cmpList(la, lb)
        if #la < #lb then
            return -1
        elseif #la > #lb then
            return 1
        else
            for i = 1, #la do
                local n = cmp(la[i], lb[i])
                if n ~= 0 then
                    return n
                end
            end
            return 0
        end
    end

    function cmp(a, b)
        local ka, kb = rt:objectKind(a), rt:objectKind(b)
        if ka ~= kb then
            error("types are not equal")
        end

        local OK = ObjectKind
        if ka == OK.INT or ka == OK.FLOAT or ka == OK.CHAR or ka == OK.STRING then
            local va, vb = a.value, b.value
            if va < vb then return -1 elseif va > vb then return 1 else return 0 end
        elseif ka == OK.LIST then
            local la, lb = rt:toList(a), rt:toList(b)
            return cmpList(la, lb)
        elseif ka == OK.TUPLE then
            local la, lb = rt:toTuple(a), rt:toTuple(b)
            return cmpList(la, lb)
        elseif ka == OK.UNIT then
            return 0
        elseif ka == OK.OPTION then
            if a.name < b.name then
                return -1
            elseif a.name > b.name then
                return 1
            else
                if a.name == "Nar.Base.Array.Array#ArrayImpl" then
                    local fnIndex = rt.program.exports["Nar.Base.Array.toList"]
                    if fnIndex then
                        local clos = Object.makeClosure(fnIndex, {})
                        return cmp(rt:applyFunc(clos, { a }), rt:applyFunc(clos, { b }))
                    end
                end
                return cmpList(a.values, b.values)
            end
        elseif ka == OK.RECORD then
            local rka, rva = rt:toRecord(a)
            local rkb, rvb = rt:toRecord(b)
            if #rka < #rkb then
                return -1
            elseif #rka > #rkb then
                return 1
            end
            -- Sort key arrays alongside their value arrays for stable comparison.
            local idxA, idxB = {}, {}
            for i = 1, #rka do
                idxA[i] = i; idxB[i] = i
            end
            table.sort(idxA, function(x, y) return rka[x] < rka[y] end)
            table.sort(idxB, function(x, y) return rkb[x] < rkb[y] end)
            for i = 1, #rka do
                local kxa, kxb = rka[idxA[i]], rkb[idxB[i]]
                if kxa ~= kxb then
                    return kxa < kxb and -1 or 1
                end
            end
            for i = 1, #rka do
                local c = cmp(rva[idxA[i]], rvb[idxB[i]])
                if c ~= 0 then return c end
            end
            return 0
        elseif ka == OK.FUNCTION or ka == OK.CLOSURE then
            local ia = a.fnIndex or a.arity or 0
            local ib = b.fnIndex or b.arity or 0
            if ia < ib then return -1 elseif ia > ib then return 1 else return 0 end
        elseif ka == OK.NATIVE then
            if a.ptr == b.ptr then return 0 end
            local la = type(a.ptr) == "table" and #a.ptr or 0
            local lb = type(b.ptr) == "table" and #b.ptr or 0
            if la < lb then return -1 else return 1 end
        else
            error("unsupported kind for comparison")
        end
    end

    -- === Nar.Base.Basics ===
    rt:registerDef("Nar.Base.Basics", "eq", function(rt, a, b)
        return rt:makeBool(cmp(a, b) == 0)
    end, 2)

    rt:registerDef("Nar.Base.Basics", "neq", function(rt, a, b)
        return rt:makeBool(cmp(a, b) ~= 0)
    end, 2)

    rt:registerDef("Nar.Base.Basics", "lt", function(rt, a, b)
        return rt:makeBool(cmp(a, b) < 0)
    end, 2)

    rt:registerDef("Nar.Base.Basics", "gt", function(rt, a, b)
        return rt:makeBool(cmp(a, b) > 0)
    end, 2)

    rt:registerDef("Nar.Base.Basics", "le", function(rt, a, b)
        return rt:makeBool(cmp(a, b) <= 0)
    end, 2)

    rt:registerDef("Nar.Base.Basics", "ge", function(rt, a, b)
        return rt:makeBool(cmp(a, b) >= 0)
    end, 2)

    rt:registerDef("Nar.Base.Basics", "not", function(rt, x)
        return rt:makeBool(not rt:toBool(x))
    end, 1)

    rt:registerDef("Nar.Base.Basics", "and", function(rt, x, y)
        return rt:makeBool(rt:toBool(x) and rt:toBool(y))
    end, 2)

    rt:registerDef("Nar.Base.Basics", "or", function(rt, x, y)
        return rt:makeBool(rt:toBool(x) or rt:toBool(y))
    end, 2)

    rt:registerDef("Nar.Base.Basics", "xor", function(rt, x, y)
        local a, b = rt:toBool(x), rt:toBool(y)
        return rt:makeBool((a and not b) or (not a and b))
    end, 2)

    -- === Nar.Base.Math ===
    rt:registerDef("Nar.Base.Math", "add", function(rt, x, y)
        local OK = ObjectKind
        if rt:objectKind(x) ~= rt:objectKind(y) then error("types are not equal") end
        local kind = rt:objectKind(x)
        local v = x.value + y.value
        if kind == OK.INT then
            return rt:makeInt(v)
        elseif kind == OK.FLOAT then
            return rt:makeFloat(v)
        else
            error("unsupported kind")
        end
    end, 2)

    rt:registerDef("Nar.Base.Math", "sub", function(rt, x, y)
        local OK = ObjectKind
        if rt:objectKind(x) ~= rt:objectKind(y) then error("types are not equal") end
        local kind = rt:objectKind(x)
        local v = x.value - y.value
        if kind == OK.INT then
            return rt:makeInt(v)
        elseif kind == OK.FLOAT then
            return rt:makeFloat(v)
        else
            error("unsupported kind")
        end
    end, 2)

    rt:registerDef("Nar.Base.Math", "mul", function(rt, x, y)
        local OK = ObjectKind
        if rt:objectKind(x) ~= rt:objectKind(y) then error("types are not equal") end
        local kind = rt:objectKind(x)
        local v = x.value * y.value
        if kind == OK.INT then
            return rt:makeInt(v)
        elseif kind == OK.FLOAT then
            return rt:makeFloat(v)
        else
            error("unsupported kind")
        end
    end, 2)

    rt:registerDef("Nar.Base.Math", "div", function(rt, x, y)
        local OK = ObjectKind
        if rt:objectKind(x) ~= rt:objectKind(y) then error("types are not equal") end
        if rt:objectKind(x) == OK.INT then
            if y.value == 0 then return rt:makeInt(0 / 0) end
            -- JS uses `| 0` truncation; Lua // is floor division. Use trunc.
            local q = x.value / y.value
            if q >= 0 then return rt:makeInt(math.floor(q)) end
            return rt:makeInt(-math.floor(-q))
        else
            return rt:makeFloat(x.value / y.value)
        end
    end, 2)

    rt:registerDef("Nar.Base.Math", "neg", function(rt, x)
        if rt:objectKind(x) == ObjectKind.INT then
            return rt:makeInt(-x.value)
        else
            return rt:makeFloat(-x.value)
        end
    end, 1)

    rt:registerDef("Nar.Base.Math", "abs", function(rt, x)
        if x.value >= 0 then
            return x
        else
            if rt:objectKind(x) == ObjectKind.INT then
                return rt:makeInt(-x.value)
            else
                return rt:makeFloat(-x.value)
            end
        end
    end, 1)

    rt:registerDef("Nar.Base.Math", "toPower", function(rt, pow, num)
        if rt:objectKind(pow) ~= rt:objectKind(num) then error("types are not equal") end
        local kind = rt:objectKind(num)
        local v = num.value ^ pow.value
        if kind == ObjectKind.INT then
            return rt:makeInt(v)
        else
            return rt:makeFloat(v)
        end
    end, 2)

    rt:registerDef("Nar.Base.Math", "isNan", function(rt, n)
        return rt:makeBool(n.value ~= n.value)
    end, 1)

    rt:registerDef("Nar.Base.Math", "isInf", function(rt, n)
        local v = n.value
        return rt:makeBool(v == math.huge or v == -math.huge)
    end, 1)

    rt:registerDef("Nar.Base.Math", "toFloat", function(rt, n)
        return rt:makeFloat(tonumber(n.value) or 0)
    end, 1)

    rt:registerDef("Nar.Base.Math", "round", function(rt, n)
        return rt:makeInt(math.floor(n.value + 0.5))
    end, 1)

    rt:registerDef("Nar.Base.Math", "floor", function(rt, n)
        return rt:makeInt(math.floor(n.value))
    end, 1)

    rt:registerDef("Nar.Base.Math", "ceil", function(rt, n)
        return rt:makeInt(math.ceil(n.value))
    end, 1)

    rt:registerDef("Nar.Base.Math", "trunc", function(rt, n)
        return rt:makeInt(math.floor(n.value > 0 and n.value or -n.value) * (n.value > 0 and 1 or -1))
    end, 1)

    rt:registerDef("Nar.Base.Math", "sqrt", function(rt, n)
        return rt:makeFloat(math.sqrt(n.value))
    end, 1)

    rt:registerDef("Nar.Base.Math", "remainderBy", function(rt, n, x)
        if n.value == 0 then return rt:makeInt(0 / 0) end
        -- JS `%` is trunc-mod (result has sign of dividend); Lua `%` is floor-mod.
        return rt:makeInt(math.floor(math.fmod(x.value, n.value)))
    end, 2)

    rt:registerDef("Nar.Base.Math", "modBy", function(rt, modulus, x)
        if modulus.value == 0 then return rt:makeInt(0) end
        local answer = x.value % modulus.value
        if (answer > 0 and modulus.value < 0) or (answer < 0 and modulus.value > 0) then
            return rt:makeInt(answer + modulus.value)
        else
            return rt:makeInt(answer)
        end
    end, 2)

    rt:registerDef("Nar.Base.Math", "logBase", function(rt, base, n)
        return rt:makeFloat(math.log(n.value) / math.log(base.value))
    end, 2)

    -- === Nar.Base.Bitwise ===
    -- Arithmetic right shift emulated on top of Lua 5.3+ logical `>>`.
    local function arshift(x, n)
        if x < 0 then
            return ~((~x) >> n)
        else
            return x >> n
        end
    end

    rt:registerDef("Nar.Base.Bitwise", "and", function(rt, x, y)
        return rt:makeInt(x.value & y.value)
    end, 2)

    rt:registerDef("Nar.Base.Bitwise", "or", function(rt, x, y)
        return rt:makeInt(x.value | y.value)
    end, 2)

    rt:registerDef("Nar.Base.Bitwise", "xor", function(rt, x, y)
        return rt:makeInt(x.value ~ y.value)
    end, 2)

    rt:registerDef("Nar.Base.Bitwise", "complement", function(rt, x)
        return rt:makeInt(~x.value)
    end, 1)

    rt:registerDef("Nar.Base.Bitwise", "shiftLeftBy", function(rt, x, y)
        return rt:makeInt(y.value << x.value)
    end, 2)

    rt:registerDef("Nar.Base.Bitwise", "shiftRightBy", function(rt, x, y)
        return rt:makeInt(arshift(y.value, x.value))
    end, 2)

    rt:registerDef("Nar.Base.Bitwise", "shiftRightZfBy", function(rt, x, y)
        return rt:makeInt(y.value >> x.value)
    end, 2)

    -- === Nar.Base.Char ===
    rt:registerDef("Nar.Base.Char", "toUpper", function(rt, char)
        return rt:makeChar(string.byte(string.upper(string.char(char.value))))
    end, 1)

    rt:registerDef("Nar.Base.Char", "toLower", function(rt, char)
        return rt:makeChar(string.byte(string.lower(string.char(char.value))))
    end, 1)

    rt:registerDef("Nar.Base.Char", "toCode", function(rt, char)
        return rt:makeInt(char.value)
    end, 1)

    rt:registerDef("Nar.Base.Char", "fromCode", function(rt, code)
        return rt:makeChar(code.value)
    end, 1)

    -- === Nar.Base.Debug ===
    local valueToString
    valueToString = function(x)
        local OK = ObjectKind
        local kind = Object.getKind(x)
        if kind == OK.CHAR then
            return utf8.char(x.value)
        elseif kind == OK.STRING then
            return x.value
        elseif kind == OK.INT then
            return tostring(x.value)
        elseif kind == OK.FLOAT then
            local v = x.value
            if v ~= v then return "NaN" end
            if v == math.huge then return "Infinity" end
            if v == -math.huge then return "-Infinity" end
            local s = tostring(v)
            -- Lua prints 42.0 for floats; Nar tests don't exercise this,
            -- but keep a trailing ".0" so float/int are distinguishable.
            if not s:find("[%.eE]") then s = s .. ".0" end
            return s
        elseif kind == OK.UNIT then
            return "()"
        elseif kind == OK.LIST then
            local items = rt:toList(x)
            local parts = {}
            for i, it in ipairs(items) do parts[i] = valueToString(it) end
            return "[" .. table.concat(parts, ", ") .. "]"
        elseif kind == OK.TUPLE then
            local items = rt:toTuple(x)
            local parts = {}
            for i, it in ipairs(items) do parts[i] = valueToString(it) end
            return "(" .. table.concat(parts, ", ") .. ")"
        elseif kind == OK.RECORD then
            local keys, values = rt:toRecord(x)
            local parts = {}
            for i = 1, #keys do
                parts[i] = keys[i] .. " = " .. valueToString(values[i])
            end
            return "{" .. table.concat(parts, ", ") .. "}"
        elseif kind == OK.OPTION then
            local name, values = rt:toOption(x)
            -- Option names are stored as "Module.Type#Ctor"; render just the ctor.
            local ctor = name:match("#(.+)$") or name
            if #values == 0 then return ctor end
            local parts = {}
            for i, v in ipairs(values) do parts[i] = valueToString(v) end
            return ctor .. "(" .. table.concat(parts, ", ") .. ")"
        elseif kind == OK.NATIVE then
            return "<native>"
        elseif kind == OK.FUNCTION or kind == OK.CLOSURE then
            return "<function>"
        else
            return "<unknown>"
        end
    end

    rt:registerDef("Nar.Base.Debug", "toString", function(rt, x)
        return rt:makeString(valueToString(x))
    end, 1)

    rt:registerDef("Nar.Base.Debug", "log", function(rt, msg, a)
        io.write(rt:toString(msg) .. valueToString(a) .. "\n")
        return a
    end, 2)

    rt:registerDef("Nar.Base.Debug", "todo", function(rt, msg)
        io.stderr:write(rt:toString(msg) .. "\n")
        return rt:makeUnit()
    end, 1)

    -- === Nar.Base.String ===
    rt:registerDef("Nar.Base.String", "length", function(rt, s)
        return rt:makeInt(#rt:toString(s))
    end, 1)

    rt:registerDef("Nar.Base.String", "reverse", function(rt, s)
        local str = rt:toString(s)
        local cps = {}
        local n = 0
        for _, cp in utf8.codes(str) do
            n = n + 1; cps[n] = cp
        end
        for i = 1, n // 2 do cps[i], cps[n - i + 1] = cps[n - i + 1], cps[i] end
        return rt:makeString(utf8.char(table.unpack(cps)))
    end, 1)

    rt:registerDef("Nar.Base.String", "append", function(rt, a, b)
        return rt:makeString(rt:toString(a) .. rt:toString(b))
    end, 2)

    rt:registerDef("Nar.Base.String", "split", function(rt, sep, string)
        local str = rt:toString(string)
        local pattern = rt:toString(sep)
        local result = {}
        if #pattern == 0 then
            for i = 1, #str do
                result[i] = rt:makeString(str:sub(i, i))
            end
            return rt:makeList(result)
        end
        local startPos = 1
        while true do
            local i, j = str:find(pattern, startPos, true)
            if i == nil then
                result[#result + 1] = rt:makeString(str:sub(startPos))
                break
            end
            result[#result + 1] = rt:makeString(str:sub(startPos, i - 1))
            startPos = j + 1
        end
        return rt:makeList(result)
    end, 2)

    rt:registerDef("Nar.Base.String", "join", function(rt, sep, strings)
        local str_list = rt:toList(strings)
        local parts = {}
        for _, s in ipairs(str_list) do
            table.insert(parts, rt:toString(s))
        end
        return rt:makeString(table.concat(parts, rt:toString(sep)))
    end, 2)

    rt:registerDef("Nar.Base.String", "words", function(rt, string)
        local str = rt:toString(string):match("^%s*(.-)%s*$")
        local result = {}
        for word in str:gmatch("%S+") do
            table.insert(result, rt:makeString(word))
        end
        return rt:makeList(result)
    end, 1)

    rt:registerDef("Nar.Base.String", "lines", function(rt, string)
        local str = rt:toString(string):match("^%s*(.-)%s*$")
        local result = {}
        for line in str:gmatch("[^\n\r]+") do
            table.insert(result, rt:makeString(line))
        end
        return rt:makeList(result)
    end, 1)

    rt:registerDef("Nar.Base.String", "slice", function(rt, begin, end_, s)
        local str = rt:toString(s)
        local len = #str
        local b = rt:toInt(begin)
        local e = rt:toInt(end_)
        -- JS `String.slice`: negative indices wrap from end; clamp to length.
        if b < 0 then b = math.max(0, len + b) end
        if e < 0 then e = math.max(0, len + e) end
        if e > len then e = len end
        if b >= e then return rt:makeString("") end
        return rt:makeString(str:sub(b + 1, e))
    end, 3)

    rt:registerDef("Nar.Base.String", "contains", function(rt, sub, string)
        return rt:makeBool(rt:toString(string):find(rt:toString(sub), 1, true) ~= nil)
    end, 2)

    rt:registerDef("Nar.Base.String", "startsWith", function(rt, sub, string)
        return rt:makeBool(rt:toString(string):sub(1, #rt:toString(sub)) == rt:toString(sub))
    end, 2)

    rt:registerDef("Nar.Base.String", "endsWith", function(rt, sub, string)
        local s = rt:toString(string)
        local u = rt:toString(sub)
        return rt:makeBool(s:sub(- #u) == u)
    end, 2)

    rt:registerDef("Nar.Base.String", "toUpper", function(rt, s)
        return rt:makeString(string.upper(rt:toString(s)))
    end, 1)

    rt:registerDef("Nar.Base.String", "toLower", function(rt, s)
        return rt:makeString(string.lower(rt:toString(s)))
    end, 1)

    rt:registerDef("Nar.Base.String", "trim", function(rt, s)
        return rt:makeString(rt:toString(s):match("^%s*(.-)%s*$"))
    end, 1)

    rt:registerDef("Nar.Base.String", "trimLeft", function(rt, s)
        return rt:makeString(rt:toString(s):match("^%s*(.*)"))
    end, 1)

    rt:registerDef("Nar.Base.String", "trimRight", function(rt, s)
        return rt:makeString(rt:toString(s):match("(.-)%s*$"))
    end, 1)

    rt:registerDef("Nar.Base.String", "indices", function(rt, sub, string)
        local s = rt:toString(string)
        local u = rt:toString(sub)
        if #u == 0 then return rt:makeList({}) end
        local result = {}
        local idx = 1
        while true do
            local i = s:find(u, idx, true)
            if i == nil then break end
            result[#result + 1] = rt:makeInt(i - 1)
            idx = i + 1
        end
        return rt:makeList(result)
    end, 2)

    local MAYBE_NOTHING = "Nar.Base.Maybe.Maybe#Nothing"
    local MAYBE_JUST = "Nar.Base.Maybe.Maybe#Just"

    rt:registerDef("Nar.Base.String", "toInt", function(rt, n)
        local str = rt:toString(n)
        if #str == 0 then return rt:makeOption(MAYBE_NOTHING, {}) end
        local code0 = str:byte(1)
        local start = (code0 == 0x2B or code0 == 0x2D) and 2 or 1
        local total = 0
        local consumed = false
        for j = start, #str do
            local code = str:byte(j)
            if code < 0x30 or code > 0x39 then
                return rt:makeOption(MAYBE_NOTHING, {})
            end
            total = 10 * total + code - 0x30
            consumed = true
        end
        if not consumed then return rt:makeOption(MAYBE_NOTHING, {}) end
        if code0 == 0x2D then total = -total end
        return rt:makeOption(MAYBE_JUST, { rt:makeInt(total) })
    end, 1)

    rt:registerDef("Nar.Base.String", "fromInt", function(rt, n)
        return rt:makeString(tostring(rt:toInt(n)))
    end, 1)

    rt:registerDef("Nar.Base.String", "toFloat", function(rt, n)
        local s = rt:toString(n)
        if #s == 0 or s:find("[%sxbo]") then
            return rt:makeOption(MAYBE_NOTHING, {})
        end
        local x = tonumber(s)
        if x == nil or x ~= x then
            return rt:makeOption(MAYBE_NOTHING, {})
        end
        return rt:makeOption(MAYBE_JUST, { rt:makeFloat(x) })
    end, 1)

    rt:registerDef("Nar.Base.String", "fromFloat", function(rt, n)
        return rt:makeString(tostring(rt:toFloat(n)))
    end, 1)

    rt:registerDef("Nar.Base.String", "fromList", function(rt, chars)
        local list = rt:toList(chars)
        local parts = {}
        for i, c in ipairs(list) do
            parts[i] = utf8.char(c.value)
        end
        return rt:makeString(table.concat(parts))
    end, 1)

    rt:registerDef("Nar.Base.String", "cons", function(rt, c, s)
        return rt:makeString(utf8.char(rt:toChar(c)) .. rt:toString(s))
    end, 2)

    rt:registerDef("Nar.Base.String", "uncons", function(rt, str)
        local s = rt:toString(str)
        if #s == 0 then
            return rt:makeOption(MAYBE_NOTHING, {})
        end
        local c = utf8.codepoint(s, 1)
        local nextOffset = utf8.offset(s, 2) or (#s + 1)
        local rest = s:sub(nextOffset)
        return rt:makeOption(MAYBE_JUST, {
            rt:makeTuple({ rt:makeChar(c), rt:makeString(rest) })
        })
    end, 1)

    rt:registerDef("Nar.Base.String", "map", function(rt, f, str)
        local s = rt:toString(str)
        local parts = {}
        local n = 0
        for _, cp in utf8.codes(s) do
            local r = rt:applyFunc(f, { rt:makeChar(cp) })
            n = n + 1
            parts[n] = utf8.char(rt:toChar(r))
        end
        return rt:makeString(table.concat(parts))
    end, 2)

    rt:registerDef("Nar.Base.String", "filter", function(rt, f, str)
        local s = rt:toString(str)
        local parts = {}
        local n = 0
        for _, cp in utf8.codes(s) do
            if rt:toBool(rt:applyFunc(f, { rt:makeChar(cp) })) then
                n = n + 1
                parts[n] = utf8.char(cp)
            end
        end
        return rt:makeString(table.concat(parts))
    end, 2)

    rt:registerDef("Nar.Base.String", "foldl", function(rt, f, acc, str)
        local s = rt:toString(str)
        for _, cp in utf8.codes(s) do
            acc = rt:applyFunc(f, { rt:makeChar(cp), acc })
        end
        return acc
    end, 3)

    rt:registerDef("Nar.Base.String", "foldr", function(rt, f, acc, str)
        local s = rt:toString(str)
        local cps = {}
        local n = 0
        for _, cp in utf8.codes(s) do
            n = n + 1; cps[n] = cp
        end
        for i = n, 1, -1 do
            acc = rt:applyFunc(f, { rt:makeChar(cps[i]), acc })
        end
        return acc
    end, 3)

    rt:registerDef("Nar.Base.String", "any", function(rt, f, str)
        local s = rt:toString(str)
        for _, cp in utf8.codes(s) do
            if rt:toBool(rt:applyFunc(f, { rt:makeChar(cp) })) then
                return rt:makeBool(true)
            end
        end
        return rt:makeBool(false)
    end, 2)

    rt:registerDef("Nar.Base.String", "all", function(rt, f, str)
        local s = rt:toString(str)
        for _, cp in utf8.codes(s) do
            if not rt:toBool(rt:applyFunc(f, { rt:makeChar(cp) })) then
                return rt:makeBool(false)
            end
        end
        return rt:makeBool(true)
    end, 2)

    -- === Nar.Base.List ===
    rt:registerDef("Nar.Base.List", "cons", function(rt, head, tail)
        return rt:makeListCons(head, tail)
    end, 2)

    local function listMapN(rt, f, lists)
        local arrs = {}
        local minLen = math.huge
        for i, l in ipairs(lists) do
            arrs[i] = rt:toList(l)
            if #arrs[i] < minLen then minLen = #arrs[i] end
        end
        if minLen == math.huge then minLen = 0 end
        local r = {}
        for i = 1, minLen do
            local args = {}
            for j = 1, #arrs do args[j] = arrs[j][i] end
            r[i] = rt:applyFunc(f, args)
        end
        return rt:makeList(r)
    end

    rt:registerDef("Nar.Base.List", "map2", function(rt, f, a, b)
        return listMapN(rt, f, { a, b })
    end, 3)

    rt:registerDef("Nar.Base.List", "map3", function(rt, f, a, b, c)
        return listMapN(rt, f, { a, b, c })
    end, 4)

    rt:registerDef("Nar.Base.List", "map4", function(rt, f, a, b, c, d)
        return listMapN(rt, f, { a, b, c, d })
    end, 5)

    rt:registerDef("Nar.Base.List", "map5", function(rt, f, a, b, c, d, e)
        return listMapN(rt, f, { a, b, c, d, e })
    end, 6)

    rt:registerDef("Nar.Base.List", "sortWith", function(rt, f, xs)
        local l = rt:toList(xs)
        table.sort(l, function(a, b)
            local res = rt:applyFunc(f, { a, b })
            if res.name == "Nar.Base.Basics.Order#LT" then return true end
            if res.name == "Nar.Base.Basics.Order#GT" then return false end
            if res.name == "Nar.Base.Basics.Order#EQ" then return false end
            error("expected Nar.Base.Basics.Order")
        end)
        return rt:makeList(l)
    end, 2)

    rt:registerDef("Nar.Base.List", "sortBy", function(rt, f, xs)
        local l = rt:toList(xs)
        table.sort(l, function(a, b)
            local xa = rt:applyFunc(f, { a })
            local xb = rt:applyFunc(f, { b })
            return cmp(xa, xb) < 0
        end)
        return rt:makeList(l)
    end, 2)

    -- === Nar.Base.Debug.getType ===
    -- (not present in the JS reference; Debug.nar declares it as native and
    -- Nar.Tests/Expect.nar uses it, so map ObjectKind → Debug.Type option.)
    local TYPE_OPTION = {
        [ObjectKind.UNKNOWN] = "Nar.Base.Debug.Type#TypeUnknown",
        [ObjectKind.UNIT] = "Nar.Base.Debug.Type#TypeUnit",
        [ObjectKind.INT] = "Nar.Base.Debug.Type#TypeInt",
        [ObjectKind.FLOAT] = "Nar.Base.Debug.Type#TypeFloat",
        [ObjectKind.STRING] = "Nar.Base.Debug.Type#TypeString",
        [ObjectKind.CHAR] = "Nar.Base.Debug.Type#TypeChar",
        [ObjectKind.RECORD] = "Nar.Base.Debug.Type#TypeRecord",
        [ObjectKind.TUPLE] = "Nar.Base.Debug.Type#TypeTuple",
        [ObjectKind.LIST] = "Nar.Base.Debug.Type#TypeList",
        [ObjectKind.OPTION] = "Nar.Base.Debug.Type#TypeOption",
        [ObjectKind.FUNCTION] = "Nar.Base.Debug.Type#TypeFunction",
        [ObjectKind.CLOSURE] = "Nar.Base.Debug.Type#TypeClosure",
        [ObjectKind.NATIVE] = "Nar.Base.Debug.Type#TypeNative",
    }
    rt:registerDef("Nar.Base.Debug", "getType", function(rt, x)
        local name = TYPE_OPTION[rt:objectKind(x)] or "Nar.Base.Debug.Type#TypeUnknown"
        return rt:makeOption(name, {})
    end, 1)

    -- === Nar.Base.NativeArray ===
    local EMPTY_ARRAY = rt:makeNative({})

    rt:registerDef("Nar.Base.NativeArray", "empty", function(_rt)
        return EMPTY_ARRAY
    end, 0)

    rt:registerDef("Nar.Base.NativeArray", "singleton", function(rt, item)
        return rt:makeNative({ item })
    end, 1)

    rt:registerDef("Nar.Base.NativeArray", "length", function(rt, array)
        return rt:makeInt(#array.ptr)
    end, 1)

    rt:registerDef("Nar.Base.NativeArray", "initialize", function(rt, size, offset, func)
        local n = rt:toInt(size)
        local off = rt:toInt(offset)
        local result = {}
        for i = 1, n do
            result[i] = rt:applyFunc(func, { rt:makeInt(off + i - 1) })
        end
        return rt:makeNative(result)
    end, 3)

    rt:registerDef("Nar.Base.NativeArray", "initializeFromList", function(rt, max, ls)
        local maxV = rt:toInt(max)
        local result = {}
        local current = ls
        local i = 0
        while i < maxV and getmetatable(current) == Object.META.LIST do
            i = i + 1
            result[i] = current.value
            current = current.next
        end
        if current == nil then current = rt:makeEmptyList() end
        return rt:makeTuple({ rt:makeNative(result), current })
    end, 2)

    rt:registerDef("Nar.Base.NativeArray", "unsafeGet", function(_rt, index, array)
        return array.ptr[index.value + 1]
    end, 2)

    rt:registerDef("Nar.Base.NativeArray", "unsafeSet", function(rt, index, value, array)
        local src = array.ptr
        local result = {}
        for i = 1, #src do result[i] = src[i] end
        result[index.value + 1] = value
        return rt:makeNative(result)
    end, 3)

    rt:registerDef("Nar.Base.NativeArray", "push", function(rt, value, array)
        local src = array.ptr
        local n = #src
        local result = {}
        for i = 1, n do result[i] = src[i] end
        result[n + 1] = value
        return rt:makeNative(result)
    end, 2)

    rt:registerDef("Nar.Base.NativeArray", "foldl", function(rt, func, acc, array)
        local src = array.ptr
        for i = 1, #src do
            acc = rt:applyFunc(func, { src[i], acc })
        end
        return acc
    end, 3)

    rt:registerDef("Nar.Base.NativeArray", "foldr", function(rt, func, acc, array)
        local src = array.ptr
        for i = #src, 1, -1 do
            acc = rt:applyFunc(func, { src[i], acc })
        end
        return acc
    end, 3)

    rt:registerDef("Nar.Base.NativeArray", "map", function(rt, func, array)
        local src = array.ptr
        local n = #src
        local result = {}
        for i = 1, n do
            result[i] = rt:applyFunc(func, { src[i] })
        end
        return rt:makeNative(result)
    end, 2)

    rt:registerDef("Nar.Base.NativeArray", "indexedMap", function(rt, func, offset, array)
        local src = array.ptr
        local off = rt:toInt(offset)
        local n = #src
        local result = {}
        for i = 1, n do
            result[i] = rt:applyFunc(func, { rt:makeInt(off + i - 1), src[i] })
        end
        return rt:makeNative(result)
    end, 3)

    rt:registerDef("Nar.Base.NativeArray", "slice", function(rt, from, to, array)
        local src = array.ptr
        local f = rt:toInt(from)
        local t = rt:toInt(to)
        -- JS Array.slice: indices are 0-based, end exclusive, negative wraps from end.
        local len = #src
        if f < 0 then f = math.max(0, len + f) end
        if t < 0 then t = math.max(0, len + t) end
        if t > len then t = len end
        local result = {}
        local idx = 0
        for i = f + 1, t do
            idx = idx + 1
            result[idx] = src[i]
        end
        return rt:makeNative(result)
    end, 3)

    rt:registerDef("Nar.Base.NativeArray", "appendN", function(rt, n, dest, source)
        local destArr = dest.ptr
        local srcArr = source.ptr
        local destLen = #destArr
        local itemsToCopy = rt:toInt(n) - destLen
        if itemsToCopy > #srcArr then itemsToCopy = #srcArr end
        if itemsToCopy < 0 then itemsToCopy = 0 end
        local result = {}
        for i = 1, destLen do result[i] = destArr[i] end
        for i = 1, itemsToCopy do result[destLen + i] = srcArr[i] end
        return rt:makeNative(result)
    end, 3)
end -- return function(rt)
