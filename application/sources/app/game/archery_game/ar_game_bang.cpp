#include "ar_game_bang.h"

#include "ar_game_meteoroid.h"
#include "ar_game_arrow.h"

ar_game_bang_t bang[NUM_BANG];

#define AR_GAME_BANG_SETUP() \
do { \
    for (uint8_t i = 0; i < NUM_BANG; i++) { \
        bang[i].visible = BLACK; \
        bang[i].action_image = AR_GAME_BANG_ACTION_IMAGE_1; \
    } \
} while (0);

#define AR_GAME_BANG_UPDATE() \
do { \
    for (uint8_t i = 0; i < NUM_BANG; i++) { \
        if (bang[i].visible == WHITE) { \
            bang[i].action_image++; \
        } \
        if (bang[i].action_image >= AR_GAME_BANG_ACTION_IMAGE_END) { \
            bang[i].action_image = AR_GAME_BANG_ACTION_IMAGE_1; \
            bang[i].visible = BLACK; \
            meteoroid[i].visible = WHITE; \
        } \
    } \
} while(0);

#define AR_GAME_BANG_RESET() \
do { \
    for (uint8_t i = 0; i < NUM_BANG; i++) { \
        bang[i].visible = BLACK; \
    } \
} while (0);

void ar_game_bang_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_BANG_SETUP: {
        APP_DBG_SIG("AR_GAME_BANG_SETUP\n");
        AR_GAME_BANG_SETUP();
    }
        break;

    case AR_GAME_BANG_UPDATE: {
        APP_DBG_SIG("AR_GAME_BANG_UPDATE\n");
        AR_GAME_BANG_UPDATE();
    }
        break;

    case AR_GAME_BANG_RESET: {
        APP_DBG_SIG("AR_GAME_BANG_RESET\n");
        AR_GAME_BANG_RESET();
    }
        break;

    default:
        break;
    }
}