#include "motion.h"
#include <string.h>

#define TERMINATION_DISTANCE to_fxp(100)
#define TERMINATION_DISTANCE_SQ to_fxp(10000)

#define ERROR(...) /*printf("Error: " __VA_ARGS__)*/

void component_begin(path_component_t *component, vec2_t *out) {
    if (component->tag == PATH_COMPONENT_ARC) {
        cpmat(*out, component->arc.start_position, 2);
    } else if (component->tag == PATH_COMPONENT_LINE_SEGMENT) {
        cpmat(*out, component->line.start, 2);
    } else {
        ERROR("Error: invalid component ID\n");
    }
}

void component_end(path_component_t *component, vec2_t *out) {
    if (component->tag == PATH_COMPONENT_ARC) {
        cpmat(*out, component->arc.end_position, 2);
    } else if (component->tag == PATH_COMPONENT_LINE_SEGMENT) {
        cpmat(*out, component->line.end, 2);
    } else {
        ERROR("Error: invalid component ID\n");
    }
}

BOOL check_path(path_component_t *components, size_t N) {
    // TODO Check line segment angles match up with start/end
    for (int i = 0; i < N - 1; i++) {
        if (components[i].tag == PATH_COMPONENT_EMPTY) {
            return TRUE;
        }

        vec2_t end_prev, begin_new;
        component_end(&components[i], &end_prev);
        component_begin(&components[i + 1], &begin_new);
        if (squared_distance(end_prev, begin_new) > to_fxp(5)) {
            return FALSE;
        }
    }
    return TRUE;
}

// Find the closest point (with angle, so pose) to `point` lying on `line`.
void closest_point_line(line_segment_t *line, fxp_t *point, fxp_t *out_pose) {
    // Find the direction of the line, and the point relative to the start
    vec2_t point_relative, end_relative;
    mat_sub(point, line->start, point_relative, 2, 1);
    mat_sub(line->end, line->start, end_relative, 2, 1);

    fxp_t sq_norm = squared_distance(line->start, line->end);

    // Take the dot product and write out the projection.
    fxp_t coeff = fxp_div(inner_product(point_relative, end_relative, 2), sq_norm);
    out_pose[0] = line->start[0] + fxp_mul(coeff, end_relative[0]);
    out_pose[1] = line->start[1] + fxp_mul(coeff, end_relative[1]);
    out_pose[2] = line->angle;
}

// Find the closest point (with angle, so pose) to `point` lying on `arc`.
void closest_point_arc(arc_t *arc, fxp_t *point, fxp_t *out) {
    // Find the current radius
    fxp_t radius_current = fxp_sqrt(squared_distance(point, arc->center));

    // Multiply the current point by the ratio of the actual radius to the current radius
    fxp_t coeff = fxp_div(arc->radius, radius_current);

    out[0] = arc->center[0] + fxp_mul(coeff, point[0] - arc->center[0]);
    out[1] = arc->center[1] + fxp_mul(coeff, point[1] - arc->center[1]);
    // TODO(Kyle): out_pose[2] = ...
}

void track_path(path_tracking_state_t *path_tracking_state, vec3_t *pose, control_t *control_out) {
    // Find the correct component
    if (path_tracking_state->path_index == path_tracking_state->path_num_components) {
        control_out->left = control_out->right = 0;
        return;
    }

    path_component_t* current_component = &(path_tracking_state->path[path_tracking_state->path_index]);

    if (current_component->tag == PATH_COMPONENT_EMPTY) {
        control_out->left = control_out->right = 0;
        return;
    }

    vec2_t current_end;
    component_end(current_component, &current_end);
    if (squared_distance(current_end, *pose) < TERMINATION_DISTANCE_SQ) {
        path_tracking_state->path_index++;
        current_component = &(path_tracking_state->path[path_tracking_state->path_index]);
    }

    // Now, we have our current component. Track it.
    vec3_t goal_pose;
    if (current_component->tag == PATH_COMPONENT_ARC) {
        // Find the closest point to our current position on an arc.
        closest_point_line(&(current_component->line), *pose, goal_pose);
    } else if (current_component->tag == PATH_COMPONENT_LINE_SEGMENT) {
        // Find the closest point to our current position on a line segment.
        closest_point_arc(&(current_component->arc), *pose, goal_pose);
    } else {
        ERROR("Invalid path");
        control_out->left = control_out->right = 0;
        return;
    }

    // Control
}

void init_line_segment(path_component_t *component, vec2_t *start, vec2_t *end, fxp_t angle) {
    if (!component) return;

    component->tag = PATH_COMPONENT_LINE_SEGMENT;
    cpmat(component->line.start, *start, 2);
    cpmat(component->line.end, *end, 2);
    component->line.angle = angle;
}

void init_arc(path_component_t *component, vec2_t *center, fxp_t radius, fxp_t start_angle, fxp_t delta_angle) {
    if (!component) return;

    component->tag = PATH_COMPONENT_ARC;
    cpmat(component->arc.center, *center, 2);
    component->arc.radius = radius;
    component->arc.start_angle = start_angle;
    component->arc.delta_angle = delta_angle;

    component->arc.start_position[0] = (*center)[0] + fxp_mul(radius, fxp_sin(start_angle));
    component->arc.start_position[1] = (*center)[1] + fxp_mul(radius, -fxp_cos(start_angle));

    component->arc.end_position[0] =
        (*center)[0] + fxp_mul(radius, fxp_sin(delta_angle + start_angle));
    component->arc.end_position[1] =
        (*center)[1] + fxp_mul(radius, -fxp_cos(delta_angle + start_angle));
}
