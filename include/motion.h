#pragma once

#include <stdint.h>
#include "linalg_math.h"
#include "fxp.h"
#include "robot_structs.h"

typedef struct {
    vec2_t center;
    fxp_t radius;
    fxp_t start_angle, delta_angle;
    vec2_t start_position, end_position;
} arc_t;

typedef struct {
    vec2_t start, end;
    fxp_t angle;
} line_segment_t;

typedef struct {
    enum {
        PATH_COMPONENT_EMPTY,
        PATH_COMPONENT_ARC,
        PATH_COMPONENT_LINE_SEGMENT,
    } tag;
    union {
        arc_t arc;
        line_segment_t line;
    };
} path_component_t;

typedef struct {
    path_component_t *path;
    size_t path_num_components;
    size_t path_index;
} path_tracking_state_t;

void init_line_segment(path_component_t *component, vec2_t *start, vec2_t *end, fxp_t angle);
void init_arc(path_component_t *component, vec2_t *center, fxp_t radius, fxp_t start_angle, fxp_t delta_angle);

BOOL check_path(path_component_t *components, size_t N);
void track_path(path_tracking_state_t *path_tracking_state, vec3_t *pose, control_t *control_out);
