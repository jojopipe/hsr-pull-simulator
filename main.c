#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include <stdint.h>
#include <time.h>

#define user_input_buf_size 128

#define action_count 5
char *possible_actions[action_count] = {"help", "single", "ten", "exit", "pity"};

typedef struct pull_result {
    uint8_t stars;
    uint8_t is_character;
    uint8_t unit_id;
} pull_result_t;

typedef struct pull_stats {
    pull_result_t *pulls;
    uint8_t pity_4star;
    uint8_t pity_5star;
} pull_stats_t;

void get_user_input(char *input_buf) {
    printf("action: \n");
    fgets(input_buf, user_input_buf_size, stdin);
    if (!input_buf) { /* user likely pressed ^D */ // does this even work on windows?? idk
        exit(69);
    }
}

int get_action_id(char *user_input) {
    for (int i = 0; i < action_count; ++i) {
        if (strcmp(user_input, possible_actions[i]) == 0)
            return i;
    }
    return -1;
}

void action_help(void) {
    printf("possible commands:\n");
    for (int i = 0; i < action_count; ++i) {
        printf("[%d] %s\n", i, possible_actions[i]);
    }
    printf("type either the [command id] or the command itself.\n");
}
void action_single_pull(pull_stats_t *stats) {
        pull_result_t pull;
    pull.stars = 3;
    int guarantee_4star = 0;
    int guarantee_5star = 0;
    float prob_4star = 0.051f;
    float prob_5star = 0.006f;
    // look at 4 star pity
    // if pity4 == 9 -> guarantee 4 star
    // if pity5 >= 74 -> increase 5 star %
    // if pity5 == 89 -> guarantee 5 star
    // calculate probability
    // do gacha
    // if 3 star -> increase both pities
    // if 4 star -> reset pity4 and increase pity5
    // if 5 star -> increase pity4 and reset pity5
    // add pull to pull history
    if (stats->pity_4star == 9) {
        guarantee_4star = 1;
        pull.stars = 4;
        stats->pity_4star = 0;
    }
    if (stats->pity_5star >= 74) {
        prob_5star += (float) (stats->pity_4star - 73) * 0.06f;
    }
    if (stats->pity_5star == 89) {
        guarantee_5star = 1;
        pull.stars = 5;
    }

    srand(time(NULL)); //predictable sequence is fine, since we only need 1 value
    double random_value = (double) rand() / RAND_MAX;
    if (random_value <= prob_4star) {
        //pulled a 4 star
    }
    else if (random_value <= prob_4star + prob_5star) {
        //pulled a 5 star
    }

}
void action_ten_pull(pull_stats_t *stats) {

}
void action_exit(void) {
    exit(69);
}

void action_show_pity(pull_stats_t *stats) {

}

void main_loop(void) {
    int program_should_end = 0;
    pull_stats_t stats;
    stats.pity_4star = 0;
    stats.pity_5star = 0;
    stats.pulls = (pull_result_t*) calloc(100, sizeof(pull_result_t));
    while (!program_should_end) {
        char user_input[user_input_buf_size];
        get_user_input(user_input);
        int action_id;

        int direct_id = (int) strtol(user_input, NULL, 10);
        if (0 <= direct_id && direct_id <= action_count) {
            action_id = direct_id;
        } else {
            action_id = get_action_id(user_input);
        }
        switch (action_id) {
            case 0:
                action_help();
                break;
            case 1:
                action_single_pull(&stats);
                break;
            case 2:
                action_ten_pull(&stats);
                break;
            case 3:
                program_should_end = 1;
                continue;
            case 4:
                action_show_pity(&stats);
                break;
            default:
                printf("unknown command\n");
                break;
        }
    }
    free(stats.pulls);
}

int main(void) {
    main_loop();
    return 0;
}