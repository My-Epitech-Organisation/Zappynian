/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** World utility functions
*/

#include "../include/world.h"
#include "../include/server.h"

int get_resource_type_from_name(const char *resource_name)
{
    const char **resource_names = get_resource_names();

    if (!resource_name)
        return -1;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcmp(resource_name, resource_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool take_resource_from_tile(tile_t *tile, int resource_type)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return false;
    if (tile->resources[resource_type] > 0) {
        tile->resources[resource_type]--;
        return true;
    }
    return false;
}

bool set_resource_on_tile(tile_t *tile, int resource_type)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return false;
    tile->resources[resource_type]++;
    return true;
}

void normalize_coordinates_toroidal(int *x, int *y, size_t width,
    size_t height)
{
    if (!x || !y)
        return;
    while (*x < 0)
        *x += width;
    while (*y < 0)
        *y += height;
    while ((size_t)*x >= width)
        *x -= width;
    while ((size_t)*y >= height)
        *y -= height;
}

int calculate_toroidal_distance(int x1, int y1, int x2, int y2,
    size_t width, size_t height)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    dx = (dx < (int)width - dx) ? dx : (int)width - dx;
    dy = (dy < (int)height - dy) ? dy : (int)height - dy;
    return dx + dy;
}
