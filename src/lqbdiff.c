#include <stdlib.h>
#include <stdint.h>

#include "lua.h"
#include "lauxlib.h"

#include "libqbdiff.h"

#if defined(_WIN32) || defined(_WIN64)
#define DLLEXPORT __declspec(dllexport)
#elif
#define DLLEXPORT
#endif /* _WIN32 */

static int
lcompute(lua_State *L)
{
    if (lua_gettop(L) != 3)
    {
        return luaL_error(L, "3 args is needed, old data, new data and diff file name");
    }
    size_t old_size, new_size;
    const char *old_data = luaL_checklstring(L, 1, &old_size);
    const char *new_data = luaL_checklstring(L, 2, &new_size);
    const char *difffile = luaL_checkstring(L, 3);
    FILE *f = fopen(difffile, "wb");
    if (f == NULL)
    {
        return luaL_error(L, "can not open difffile %s", difffile);
    }

    int ret = qbdiff_compute((const uint8_t *) old_data, (const uint8_t *) new_data, old_size, new_size, f);
    fflush(f);
    fclose(f);
    if (ret != QBERR_OK)
    {
        return luaL_error(L, qbdiff_error(ret));
    }
    lua_pushinteger(L, (lua_Integer) ret);
    return 1;
}

static int
lpatch(lua_State *L)
{
    if (lua_gettop(L) != 3)
    {
        return luaL_error(L, "3 args is needed, old data, patch data and new file name");
    }
    size_t old_size, patch_size;
    const char *old_data = luaL_checklstring(L, 1, &old_size);
    const char *patch_data = luaL_checklstring(L, 2, &patch_size);
    const char *newfile = luaL_checkstring(L, 3);
    FILE *f = fopen(newfile, "wb");
    if (f == NULL)
    {
        return luaL_error(L, "can not open difffile %s", newfile);
    }

    int ret = qbdiff_patch((const uint8_t *) old_data, (const uint8_t *) patch_data, old_size, patch_size, f);
    fflush(f);
    fclose(f);
    if (ret != QBERR_OK)
    {
        return luaL_error(L, qbdiff_error(ret));
    }
    lua_pushinteger(L, (lua_Integer) ret);
    return 1;
}

static int
lerror(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "one error code must be given");
    }
    int code = (int) luaL_checkinteger(L, 1);
    const char *errmsg = qbdiff_error(code);
    lua_pushstring(L, errmsg);
    return 1;
}

static luaL_Reg lua_funcs[] = {
        {"compute", &lcompute},
        {"patch",   &lpatch},
        {"error",   &lerror},
        {NULL, NULL}
};

DLLEXPORT int luaopen_qbdiff(lua_State *L)
{
    luaL_newlib(L, lua_funcs);
#define LSETCONST(name) lua_pushinteger(L, name); \
    lua_setfield(L, -2, #name);

    LSETCONST(QBERR_OK)
    LSETCONST(QBERR_NOMEM)
    LSETCONST(QBERR_IOERR)
    LSETCONST(QBERR_TRUNCPATCH)
    LSETCONST(QBERR_BADPATCH)
    LSETCONST(QBERR_BADCKSUM)
    LSETCONST(QBERR_LZMAERR)
    LSETCONST(QBERR_SAIS)
//    lua_pushinteger(L, QBERR_OK );
//    lua_setfield(L, -2, "QBERR_OK");
    lua_pushstring(L, qbdiff_version());
    lua_setfield(L, -2, "version");
    return 1;
}