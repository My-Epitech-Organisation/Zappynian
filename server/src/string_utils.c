/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** string_utils
*/

#include "../include/commands.h"

bool safe_strcat(char *dest, const char *src, size_t dest_size)
{
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);

    if (dest_len + src_len + 1 > dest_size)
        return false;
    strcat(dest, src);
    return true;
}
