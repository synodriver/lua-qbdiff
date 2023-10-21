## lua binding for [qbdiff](https://github.com/synodriver/qbdiff)

### Usage

```lua
local qbdiff = require("qbdiff")

print(qbdiff.version)
print(qbdiff.error(1))

local old_data = "12345"
local new_data = "123456"

qbdiff.compute(old_data, new_data, "diff.bin")
local f = io.open("diff.bin", "rb")
local diffdata = f:read("a")
f:close()
qbdiff.patch(old_data, diffdata, "newfile.bin")

f = io.open("newfile.bin", "rb")
assert(f:read("a")==new_data)
f:close()
```