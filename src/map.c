/* Copyright (c) 2006-2014 Jonas Fonseca <jonas.fonseca@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "tig/tig.h"
#include "tig/map.h"

#include "compat/hashtab.h"


string_map_hash_fn string_map_hash_helper = htab_hash_string;

static int
string_map_equal(const void *element, const void *map_)
{
	struct string_map *map = (struct string_map *) map_;
	const char *key = map->key_fn(element);

	return !strcmp(key, map->key);
}

void *
string_map_get(struct string_map *map, const char *key)
{
	if (map->htab) {
		map->key = key;
		return htab_find_with_hash(map->htab, map, htab_hash_string(key));
	}

	return NULL;
}

void **
string_map_put(struct string_map *map, const char *key)
{
	if (!map->htab) {
		map->htab = htab_create_alloc(16, map->hash_fn, string_map_equal, NULL, calloc, free);
		if (!map->htab)
			return NULL;
	}

	map->key = key;
	return htab_find_slot_with_hash(map->htab, map, htab_hash_string(key), INSERT);
}

void
string_map_clear(struct string_map *map)
{
	if (map->htab)
		htab_empty(map->htab);
}

/* vim: set ts=8 sw=8 noexpandtab: */
