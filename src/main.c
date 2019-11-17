#include <math.h>

#include "ff14/ff.h"
#include "hardware.h"
#include "lcd.h"
#include "util.h"
#include "fxp.h"
#include "de2.h"
#include "motion.h"
#include "localization.h"

char *nothing();

void halt();

unsigned basic_pref[] = {0, 0, 0, 0};

char *t = "abcdefghijk";

volatile int a = 0;

void collect_data() {

  char foo[20];
  sonar_enable(0xFF);

  lcd_flash("Colllecting:");

  while (SWITCHES&1 || 1) {

    int x = sonar_read(0);
    int r = sprint_int(foo, x);
    foo[r] = '\n';
    foo[r + 1] = '\0';

    lcd_puts(foo);
    SEG = fxp_sin(x);
  }

}

void test_pathing() {
    path_component_t components[8];
    vec2_t start, end, center;
    {
        start[0] = 0; start[1] = 0;
        end[0] = to_fxp(500); end[1] = 0;
        fxp_t angle = 0;
        init_line_segment(&components[0], &start, &end, angle);
    }

    {
        center[0] = to_fxp(500); center[1] = to_fxp(250);
        fxp_t radius = to_fxp(250);
        fxp_t angle_start = 0, angle_delta = to_fxp(M_PI / 2);
        init_arc(&components[1], &center, radius, angle_start, angle_delta);
    }

    components[2].tag = PATH_COMPONENT_EMPTY;

    if (!check_path(components, 8)) {
        lcd_flash("Failure: invalid path");
    }

    path_tracking_state_t track_state;
    track_state.path = components;
    track_state.path_num_components = 8;
    track_state.path_index = 0;

    state_t localization_state;

    {
        for (int i = 0; i < 3; i++) {
            localization_state.pose[i] = 0;
            for (int j = 0; j < 3; j++) {
                localization_state.covariance[i * 3 + j] = 0;
            }
        }
    }

    reset_encoders();

    encoders_t prev_encoders;
    prev_encoders.left = prev_encoders.right = 0;

    while (TRUE) {
        encoders_t encoders, encoders_diffs;
        encoders.left = left_encoder_read();
        encoders.right = -right_encoder_read();
        encoders_diffs.left = encoders.left - prev_encoders.left;
        encoders_diffs.right = encoders.right - prev_encoders.right;
        prev_encoders = encoders;

        control_t control;

        predict(&localization_state, &encoders);
        // track_path(&track_state, &localization_state.pose, &control);

        motors_write(50, 200);
        sad_wait(40);
    }
}

fxp_t angles[8] = {
    to_fxp(M_PI / 180.0 * -144),
    to_fxp(M_PI / 180.0 * -90),
    to_fxp(M_PI / 180.0 * -44),
    to_fxp(M_PI / 180.0 * -12),
    to_fxp(M_PI / 180.0 * 12),
    to_fxp(M_PI / 180.0 * 44),
    to_fxp(M_PI / 180.0 * 90),
    to_fxp(M_PI / 180.0 * 144),
};

static state_t localization_state;

void odometry(void) {
    encoders_t encoders;
    encoders->left = left_encoder_read();
    encoders->right = right_encoder_read();
    predict(&localization_state, encoders);
}

#define BEACON_MAX 500

void bounce() {
    // Bounce off the walls!
    //
    // Check our position + direction * 0.3m against each wall.
    // If it intersects turn 90 degrees and continue

}

void drive_until_beacon(fxp_t *angle, fxp_t *distance) {
    while (TRUE) {
        odometry();

        bounce();

        // Ignore first and last (rear-pointing) sensors
        motors_write(300, 300);
        for (int i = 1; i < 7; i++) {
            uint32_t sonar = sonar_read(i);
            if (sonar < BEACON_MAX) {
                *distance = sonar << 16;
                *angle = angles[i];
                return;
            }
        }
    }
}

#define ticks_per_rad ((int) (9550 * 4 * 300.0 / 101.6))

void turn(fxp_t angle) {
    int diff_initial = right_encoder_read() - left_encoder_read();
    // Divide by 8 before to get better range
    int diff_goal = diff_initial + (angle * ticks_per_rad) >> 16;

    while (TRUE) {
        odometry();

        int diff = right_encoder_read() - left_encoder_read();
        int error = diff_goal - diff;
        int ff = error > 0 ? 100 : -100;
        motors_write(-error / 100 - ff, error / 100 + ff);

        if (error < 1000) {
            return;
        }
    }
}

void circle_reflector(fxp_t distance) {
    int diff_initial = right_encoder_read() - left_encoder_read();
    int diff_goal = diff_initial + (int) (2 * M_PI * ticks_per_rad);

    while (TRUE) {
        odometry();

        motors_write(300, 200);
        int diff = right_encoder_read() - left_encoder_read();

        if (diff < diff_goal) {
            return;
        }
    }
}

void stop(void) {
    motors_write(0, 0);
    for (int i = 0; i < 250; i++) {
        odometry();
        sad_wait(1);
    }
}

void circling_but_worse() {
    while (TRUE) {
        fxp_t angle, distance;
        drive_until_beacon(&angle, &distance);

        stop();

        fxp_t turn_angle = to_fxp(M_PI / 2) - angle;
        turn(turn_angle);

        stop();

        circle_reflector(distance);

        turn(to_fxp(M_PI / 2));
    }
}

int main() {
  SEG = 0xdeadbeef;

  lcd_puts("LCD init\n");

  if (fxp_mul(12345, 6789) < 10) {
    lcd_flash("HI");
  }

  circling_but_worse();

/*   *((fxp_t *)0x100000) = fxp_div(int2fxp(1), int2fxp(-1)); */
/*   *((fxp_t *)0x100004) = 0xcafebabe; */
/*   *((fxp_t *)0x100008) = 0xdeadbeef; */
/*   *((fxp_t *)0x10000C) = 0xcafebabe; */
/*   volatile fxp_t *out = (fxp_t *)0x100010; */
/*   mat2x2_t A = { */
/*       to_fxp(0.5), to_fxp(-0.5), */
/*       to_fxp(1.0), to_fxp(-3.0) */
/*   }; */
/*   SEG = 0x13371337; */
/*   mat2x2_inv(A, out); */
/*   SEG = 0x13381338; */

  /* test_pathing(); */

  /* collect_data(); */

  halt();
}

