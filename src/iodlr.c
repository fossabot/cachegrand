#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "misc.h"
#include "exttypes.h"
#include "log/log.h"
#include "iodlr.h"
#include "large_page.h"

#define TAG "iodlr-hugepages"

bool iodlr_can_enable() {
    map_status status;
    bool is_enabled;

    status = IsExplicitHugePagesEnabled(&is_enabled);
    if (status != map_ok) {
        LOG_W(TAG, "Failed to check if explicit hugepages are enabled because <%s>", MAP_STATUS_STR(status));
        return false;
    }

    if (!is_enabled) {
        status = IsTransparentHugePagesEnabled(&is_enabled);
        if (status != map_ok) {
            LOG_W(TAG, "Failed to check if transparent hugepages are enabled because <%s>", MAP_STATUS_STR(status));
            return false;
        }
    }

    return true;
}

bool iodlr_try_setup(bool use_explicit_huge_pages) {
    map_status status;

    SetUseEHP(use_explicit_huge_pages);

    status = MapStaticCodeToLargePages();
    if (status != map_ok) {
        LOG_W(TAG, "Failed to map because <%s>", MAP_STATUS_STR(status));
        return false;
    }

    return true;
}
