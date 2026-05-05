#include "ar_game_meteoroid.h"

#include "ar_game_arrow.h"
#include "ar_game_bang.h"
#include "ar_game_border.h"
#include "scr_archery_game.h"

ar_game_meteoroid_t meteoroid[NUM_METEOROIDS];

#define AR_GAME_METEOROID_SETUP() \
do { \
    for (uint8_t i = 0; i < NUM_METEOROIDS; i++) { \
        meteoroid[i].y = AXIS_Y_METEOROID_START + (i * AXIS_Y_METEOROID_STEP); \
        meteoroid[i].x = (rand() % 39) + 130; \
        meteoroid[i].visible = WHITE; \
        meteoroid[i].action_image = rand() % AR_GAME_METEOROID_ACTION_IMAGE_3 + AR_GAME_METEOROID_ACTION_IMAGE_1; \
    } \
} while (0);

#define AR_GAME_METEOROID_RUN() \
do { \
    for (uint8_t i = 0; i < NUM_METEOROIDS; i++) { \
        if (meteoroid[i].visible == WHITE) { \
            meteoroid[i].x -= settingsetup.meteoroid_speed; \
            meteoroid[i].action_image++; \
            if (meteoroid[i].action_image > AR_GAME_METEOROID_ACTION_IMAGE_3) { \
                meteoroid[i].action_image = AR_GAME_METEOROID_ACTION_IMAGE_1; \
            } \
        } \
    } \
} while(0);

#define AR_GAME_METEOROID_DETONATOR() \
do { \
    for (uint8_t i = 0; i < NUM_METEOROIDS; i++) { \
        if (meteoroid[i].visible == WHITE) { \
            for (uint8_t j = 0; j < MAX_NUM_ARROW; j++) { \
                if (arrow[j].visible == WHITE && \
                    arrow[j].x + SIZE_BITMAP_ARROW_X > meteoroid[i].x && \
                    arrow[j].x < meteoroid[i].x + SIZE_BITMAP_METEOROIDS_X && \
                    arrow[j].y + SIZE_BITMAP_ARROW_Y > meteoroid[i].y && \
                    arrow[j].y < meteoroid[i].y + SIZE_BITMAP_METEOROIDS_Y) { \
                    meteoroid[i].visible = BLACK; \
                    arrow[j].visible = BLACK; \
                    bang[i].visible = WHITE; \
                    bang[i].action_image = AR_GAME_BANG_ACTION_IMAGE_1; \
                    bang[i].x = (meteoroid[i].x > 5 ? meteoroid[i].x - 5 : 0); \
                    bang[i].y = meteoroid[i].y + 2; \
                    arrow[j].x = 0; \
                    arrow[j].y = 0; \
                    meteoroid[i].x = (rand() % 39) + 130; \
                    meteoroid[i].action_image = rand() % AR_GAME_METEOROID_ACTION_IMAGE_3 + AR_GAME_METEOROID_ACTION_IMAGE_1; \
                    if (settingsetup.num_arrow < MAX_NUM_ARROW) { \
                        settingsetup.num_arrow++; \
                    } \
                    ar_game_score += 10; \
                    BUZZER_PlayTones(tones_BUM); \
                    break; \
                } \
            } \
        } \
    } \
} while(0);

#define AR_GAME_METEOROID_RESET() \
do { \
    for (uint8_t i = 0; i < NUM_METEOROIDS; i++) { \
        meteoroid[i].visible = BLACK; \
    } \
} while (0);

void ar_game_meteoroid_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_METEOROID_SETUP: {
        APP_DBG_SIG("AR_GAME_METEOROID_SETUP\n");
        AR_GAME_METEOROID_SETUP();
    }
        break;

    case AR_GAME_METEOROID_RUN: {
        APP_DBG_SIG("AR_GAME_METEOROID_RUN\n");
        AR_GAME_METEOROID_RUN();
    }
        break;

    case AR_GAME_METEOROID_DETONATOR: {
        APP_DBG_SIG("AR_GAME_METEOROID_DETONATOR\n");
        AR_GAME_METEOROID_DETONATOR();
    }
        break;

    case AR_GAME_METEOROID_RESET: {
        APP_DBG_SIG("AR_GAME_METEOROID_RESET\n");
        AR_GAME_METEOROID_RESET();
    }
        break;

    default:
        break;
    }
}