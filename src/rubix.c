#include "rubix.h"

void even_offset(vec3 position) {
    if (RUBIX_SIZE % 2 == 0) {
        position[0] = position[0] + 0.5f * CUBE_SIZE;
        position[1] = position[1] + 0.5f * CUBE_SIZE;
        position[2] = position[2] + 0.5f * CUBE_SIZE;
    }
}

void localize(ivec3 position, vec3 out) {
    out[0] = position[0] - RUBIX_SIZE / 2;
    out[1] = position[1] - RUBIX_SIZE / 2;
    out[2] = position[2] - RUBIX_SIZE / 2;

    even_offset(out);
}

void to_world_position(Rubix *rubix, vec3 in, vec3 out) {
    glm_vec3_scale(in, CUBE_SIZE + RUBIX_GAP, out);
    glm_vec3_add(out, rubix->position, out);
}

Cube *rubix_get_cube(Rubix *rubix, vec3 local) {
    vec3 final_position;
    to_world_position(rubix, local, final_position);

    for (int i = 0; i < N_CUBES; i++) {
        Cube *c = rubix->cubes.arr[i];
        if (glm_vec3_eqv(final_position, c->position)) {
            return c;
        }
    }

    return NULL;
}

// TODO:
// - Fix dumb normalization stuff: local = -3 -> back but local 2 = front? 

Plane rubix_plane(Rubix *rubix, ptype type, int local) {
    Plane plane;
    glm_ivec3_copy(type, plane.type);

    vec3 final_position;

    int v_component;

    if (glm_ivec3_eqv(type, XY_PLANE)) {
        glm_vec3_copy((vec3) {0.0f, 0.0f, local}, final_position);
        v_component = 2;
    } else if (glm_ivec3_eqv(type, XZ_PLANE)) {
        glm_vec3_copy((vec3) {0.0f, local, 0.0f}, final_position);
        v_component = 1;
    } else if (glm_ivec3_eqv(type, YZ_PLANE)) {
        glm_vec3_copy((vec3) {local, 0.0f, 0.0f}, final_position);
        v_component = 0;
    }

    // Copy vec3 local_position into plane
    glm_vec3_copy(final_position, plane.local);

    even_offset(final_position);
    
    // This is shitty code. Right now, it's a shave and a haircut attempt at creating proper
    // local coordinates by subbing 1 from all positive values in the improper local range of say (-3, 2) (for all x)
    // so that it's in the real local range of (-3, -3) (where x !== 0)
    if (RUBIX_SIZE % 2 == 0 && final_position[v_component] >= 0.0f) {
        final_position[v_component] -= 1.0f;
    }

    to_world_position(rubix, final_position, final_position);

    int ptr = 0;
    for (int i = 0; i < N_CUBES; i++) {
        Cube *c = rubix->cubes.arr[i];
        
        float epsilon = 0.01f;
        if (fabs(c->position[v_component] - final_position[v_component]) < epsilon) {
            plane.cubes.arr[ptr] = c;
            ptr++;
        }
    }
    
    return plane;
}

static void get_pivot(Rubix *rubix, vec3 pivot) {
    Plane plane = rubix->active_plane;
    pivot[0] = plane.type[0];
    pivot[1] = plane.type[1];
    pivot[2] = plane.type[2];

    pivot[0] = -(pivot[0] - 1);
    pivot[1] = -(pivot[1] - 1);
    pivot[2] = -(pivot[2] - 1);

    // glm_vec3_add(pivot, plane.local, pivot);

    glm_vec3_divs(pivot, 2.0f, pivot);
    to_world_position(rubix, pivot, pivot);
}

Rubix *rubix_create(vec3 position) {
    Rubix *rubix = (Rubix *)malloc(sizeof(struct rubix));
    glm_vec3_copy(position, rubix->position);

    for (int i = 0; i < RUBIX_SIZE; i++) {
        for (int j = 0; j < RUBIX_SIZE; j++) {
            for (int k = 0; k < RUBIX_SIZE; k++) {
                ivec3 ijk = {i, j, k};
                vec3 final_position;

                localize(ijk, final_position);
                to_world_position(rubix, final_position, final_position);

                rubix->cubes.map[i][j][k] = cube_create(final_position, (vec3) {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE});
            }
        }
    }

    return rubix;
}

void rubix_render(Rubix *rubix, CubeRenderer *renderer) {
    for (int i = 0; i < N_CUBES; i++) {
        Cube *cube = rubix->cubes.arr[i];
        cube_renderer_draw(renderer, cube);
    }
}

void rubix_free(Rubix *rubix) {
    for (int i = 0; i < N_CUBES; i++) {
        free(rubix->cubes.arr[i]);
    }
    free(rubix);
}

Plane rotate_plane(Rubix *rubix) {
    Plane plane = rubix->active_plane;
    vec3 pivot;
    get_pivot(rubix, pivot);

    float theta = plane.theta;

    for (int i = 0; i < PLANE_SIZE; i++) {
        Cube *c = plane.cubes.arr[i];
        
        vec3 temp;
        glm_vec3_sub(c->position, pivot, temp);

        vec3 axis;
        axis[0] = -(plane.type[0] - 1);
        axis[1] = -(plane.type[1] - 1);
        axis[2] = -(plane.type[2] - 1);
        glm_vec3_copy(axis, c->axis);


        // Cap the angle
        if ((theta > 0) && (plane.angle + theta > plane.next_angle)) {
            theta = plane.next_angle - plane.angle;
        } else if ((theta < 0) && (plane.angle + theta < plane.next_angle)) {
            theta = plane.next_angle - plane.angle;
        }

        c->angle += theta;

        glm_vec3_rotate(temp, theta, axis);

        glm_vec3_add(temp, pivot, c->position);
    }

    plane.angle += theta;
    return plane;
}

void rubix_twist(Rubix *rubix, ptype type, int local, int direction) {
    Plane p = rubix->active_plane;
    
    // Make sure rubix cube is not currently being twisted 
    if (p.angle == p.next_angle) {
        rubix->active_plane = rubix_plane(rubix, type, local);
        rubix->active_plane.next_angle += direction * (GLM_PI / 2);
        rubix->active_plane.theta = direction * TWIST_SPEED;
    }
}

void rubix_rotate(Rubix *rubix) {
    Plane p = rubix->active_plane;
    
    if ((p.angle < p.next_angle && p.theta > 0) || (p.angle > p.next_angle && p.theta < 0)) {
        rubix->active_plane = rotate_plane(rubix);
    } 
}