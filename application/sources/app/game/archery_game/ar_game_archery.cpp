#include "ar_game_archery.h"

ar_game_archery_t archery;
static uint32_t archery_y;

#define AXIS_Y_ARCHERY_MIN      (10)
#define AXIS_Y_ARCHERY_MAX      (50)

#define AR_GAME_ARCHERY_SETUP() \
do { \
    archery.x = AXIS_X_ARCHERY; \
    archery.y = AXIS_Y_ARCHERY; \
    archery.visible = WHITE; \
    archery.action_image = (settingsetup.num_arrow > 0 ? \
                            AR_GAME_ARCHERY_ACTION_IMAGE_1 : \
                            AR_GAME_ARCHERY_ACTION_IMAGE_2); \
    archery_y = AXIS_Y_ARCHERY; \
} while (0);

#define AR_GAME_ARCHERY_UP() \
do { \
    if (archery_y <= STEP_ARCHERY_AXIS_Y || \
       (archery_y - STEP_ARCHERY_AXIS_Y) < AXIS_Y_ARCHERY_MIN) \
    { \
        archery_y = AXIS_Y_ARCHERY_MIN; \
    } else { \
        archery_y -= STEP_ARCHERY_AXIS_Y; \
    } \
} while(0);

#define AR_GAME_ARCHERY_DOWN() \
do { \
    archery_y += STEP_ARCHERY_AXIS_Y; \
    if (archery_y > AXIS_Y_ARCHERY_MAX) { \
        archery_y = AXIS_Y_ARCHERY_MAX; \
    } \
} while(0);

#define AR_GAME_ARCHERY_RESET() \
do { \
    archery.visible = BLACK; \
} while(0);

void ar_game_archery_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_ARCHERY_SETUP: {
        APP_DBG_SIG("AR_GAME_ARCHERY_SETUP\n");
        AR_GAME_ARCHERY_SETUP();
    }
        break;

    case AR_GAME_ARCHERY_UPDATE: {
        APP_DBG_SIG("AR_GAME_ARCHERY_UPDATE\n");
        archery.y = archery_y;
    }
        break;

    case AR_GAME_ARCHERY_UP: {
        APP_DBG_SIG("AR_GAME_ARCHERY_UP\n");
        AR_GAME_ARCHERY_UP();
    }
        break;

    case AR_GAME_ARCHERY_DOWN: {
        APP_DBG_SIG("AR_GAME_ARCHERY_DOWN\n");
        AR_GAME_ARCHERY_DOWN();
    }
        break;

    case AR_GAME_ARCHERY_RESET: {
        APP_DBG_SIG("AR_GAME_ARCHERY_RESET\n");
        AR_GAME_ARCHERY_RESET();
    }
        break;

    default:
        break;
    }
}