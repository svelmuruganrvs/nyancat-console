#include <stdlib.h>
#include "mode.h"
#include "status.h"

/* hide implementation of struct */
struct _mode {
    const char *name;
    void (*enter) (void);
    void (*leave) (void);
    void (*draw) (void);
    void (*key) (gametime_t, const int);
};

static const gamemode_t *current;

/* Flag to handle none existing concept for exit modes. For now it makes no
 * sense to saveon each mode if it's an exitmode or to use the last added mode
 * as exitmode */
static int exit_flag = 0;

/**
 * @name:   mode name that could be displayer to the user.
 * @enter:  callback function to call if mode is entered.
 * @leave:  callback function to call if mode is left.
 * @draw:   callback function to draw screen for the mode.
 * @key:    callback function to that handles keypresses.
 *
 * Creates a mode with given callbacks.
 */
gamemode_t *
mode_create(const char *name, callback enter, callback leave, callback draw,
            keyhandler key)
{

    gamemode_t *mode = (gamemode_t *) malloc(sizeof(gamemode_t));
    if (NULL == mode) {
        error_exit("Could not allocate memory");
    }

    mode->name  = name;
    mode->enter = enter ? enter : NULL;
    mode->leave = leave ? leave : NULL;
    mode->draw  = draw ? draw : NULL;
    mode->key   = key ? key : NULL;

    return mode;
}

/**
 * Set the exit mode flag to show in mode_valid that no more mode is
 * avaialable.
 */
void
mode_exit(void)
{
    exit_flag = 1;
}

/**
 * Switch into given mode. Call leave for previous mode and enter on new mode.
 */
void
mode_enter(const gamemode_t * mode)
{
    if (current && current->leave) {
        current->leave();
    }
    current = mode;
    /* collect data for status bar but draw callback decide if to write to
     * window or not */
    status_set_mode(mode->name);
    if (current->enter) {
        queue_free();
        current->enter();
    }
}

/**
 * Calls the drawing callback for current mode.
 */
void
mode_draw(void)
{
    if (current && current->draw) {
        current->draw();
    }
}

/**
 * @key: Pressed key to handle.
 *
 * Handles key presses on current modes keyhandler callback.
 */
void
mode_key(const int key)
{
    if (current && current->key) {
        current->key(clock_get_relative(), key);
    }
}

/**
 * Inidcates if there is a valid mode set.
 *
 * If no mode is set return 0 else 1.
 */
int
mode_valid(void)
{
    return !exit_flag;
}

/**
 * Retrieves the name of given mode.
 */
const char *
mode_get_name(void)
{
    if (current && current->name) {
        return current->name;
    }
    return "";
}
