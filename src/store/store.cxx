/*
 * store.cxx
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


#include "store.hpp"
#include <stdlib.h>

// Initalize some funcs

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string download(std::string download_url)
{
    CURL * curl = curl_easy_init();
    CURLcode res;
    std::string recv_data;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, download_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &recv_data);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return recv_data;
}


// Query Data
Tags get_tags()
{
    std::string recv_data = download((url + "tags").c_str());
    Tags tags;
    // interpret the JSON data.
    // Easier said than done lol
    std::cout << recv_data << std::endl;
    return tags;
}

Games search(std::string free_text)
{
    std::string recv_data = download((url + "search/free-text=" + free_text).c_str());
    Games games;
    std::cout << recv_data << std::endl;
    return games;
}

Games search(string_list tags)
{
    std::string download_url = url + "search/tags=";
    for (unsigned int i = 0; i < tags.size(); i++)
    {
        if (i == 0)
        {
            download_url = download_url + tags[i];
        }
        else
        {
            download_url = download_url + "," + tags[i];
        }
    }
    std::string recv_data = download(download_url.c_str());
    Games games;
    std::cout << recv_data << std::endl;
    return games;
}
//
// Games search(string_list tags, std::string free_text)
// {
//
// }
//
// Download_Info get_download_info(std::string pkg_name)
// {
//
// }
//
Game get_game(std::string pkg_name)
{
    std::string download_url = url + "/games/" + pkg_name;
    std::string recv_data = download(download_url.c_str());
    Game game;
    std::cout << recv_data << std::endl;
    return game;
}
//
// // Installation functions
//
// bool vstore_install_external_URL(std::string url)
// {
//
// }
//
// bool vstore_install_external_local(std::string path)
// {
//
// }
//
// bool vstore_check_installed(std::string pkg_name)
// {
//
// }
//
// bool vstore_install_pkg(std::string pkg_name)
// {
//
// }
//
// Games vstore_list_installed(std::string pkg_name)
// {
//
// }
