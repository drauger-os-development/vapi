/*
 * store.hpp
 *
 * Copyright 2021 Thomas Castleman <contact@draugeros.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
// We MIGHT need these later in order to set up the Python bindings
//#define PY_SSIZE_T_CLEAN
//#include <Python.h>
#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>

#ifndef store_hpp__
#define store_hpp__
#define elif else if
#define string_list std::vector<std::string>
#define int_list std::vector<int>
#define float_list std::vector<float>
#define bool_list std::vector<bool>

const std::string url = "http://api.store.vetala.tech/";

// Structs to emulate JSON data
// List of possible tags
struct Tags
{
    string_list genres, platforms, ratings;
};

// Data for a specific game
struct Game
{
    std::string name, description, platform, rating, screenshots_url;
    int downloads, joined;
    string_list genres;
};

// Info needed to download something
struct Download_Info
{
    std::string url;
    bool in_pack_man;
};

// List of games
#define Games std::vector<Game>


// Query Data

extern Tags get_tags();

extern Games search(std::string free_text);

extern Games search(string_list tags);

extern Games search(string_list tags, std::string free_text);

extern Download_Info get_download_info(std::string pkg_name);

extern Game get_game(std::string pkg_name);

// Installation functions

extern bool vstore_install_external_URL(std::string url);

extern bool vstore_install_external_local(std::string path);

extern bool vstore_check_installed(std::string pkg_name);

extern bool vstore_install_pkg(std::string pkg_name);

extern Games vstore_list_installed(std::string pkg_name);



#endif  // store_hpp__
