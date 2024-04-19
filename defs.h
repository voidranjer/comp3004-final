#ifndef DEFS_H
#define DEFS_H

// controller config
#define CLOCK_TICK          1000 // 1 second
#define NUM_ELECTRODES      7

// graph config
#define GRAPH_XRANGE        3000
#define GRAPH_YRANGE        1000
#define FEEDBACK_FACTOR     3   // exaggerate feedback amplitude multiplier (visual only)
#define NOISE_FACTOR        1.5 // noise amplitude multiplier

// therapy config
#define OBSERVE_DURATION    5000 // 5 seconds
#define FEEDBACK_DURATION   1000 // 1 second
#define NUM_ROUNDS          5    // 4 + 1 (last round is measurement only)

#define BATTERY_LIFE        180 // Number of seconds battery will last (x2 times used while in session)

#endif // DEFS_H
