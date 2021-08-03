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


// JSON parsing helpers
string_list parse_list(std::string input)
{
    string_list output;
    std::string append = "";
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (input[i] == '"')
        {
            if (append.size() != 0)
            {
                output.push_back(append);
                append = "";
            }
        }
        elif ((input[i] != '[') && (input[i] != ']') && (input[i] != ','))
        {
            append = append + input[i];
        }
    }
    return output;
}


Game parse_game(std::string input)
{
    Game output;
    //std::string append = "";
    unsigned int overshoot = 0;
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (overshoot != 0)
        {
            overshoot--;
            continue;
        }
        //std::cout << input.substr(i, i + 9).substr(0, 9) << std::endl;
        if (input.substr(i, i + 2) == "Name")
        {
            for (unsigned int j = i + 7; j < input.size(); j++)
            {
                if (input[j] != '"')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.name = input.substr(i + 7, i + overshoot).substr(0, overshoot);
        }
        elif (input.substr(i, i + 11).substr(0, 11) == "description")
        {
            for (unsigned int j = i + 14; j < input.size(); j++)
            {
                if (input[j] != '"')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.description = input.substr(i + 14, i + overshoot).substr(0, overshoot);
        }
        elif (input.substr(i, i + 8).substr(0, 8) == "platform")
        {
            for (unsigned int j = i + 11; j < input.size(); j++)
            {
                if (input[j] != '"')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.platform = input.substr(i + 11, i + overshoot).substr(0, overshoot);
        }
        elif (input.substr(i, i + 6).substr(0, 6) == "rating")
        {
            for (unsigned int j = i + 9; j < input.size(); j++)
            {
                if (input[j] != '"')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.rating = input.substr(i + 9, i + overshoot).substr(0, overshoot);
        }
        elif (input.substr(i, i + 15).substr(0, 15) == "screenshots_url")
        {
            for (unsigned int j = i + 18; j < input.size(); j++)
            {
                if (input[j] != '"')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.screenshots_url = input.substr(i + 18, i + overshoot).substr(0, overshoot);
        }
        elif (input.substr(i, i + 6).substr(0, 6) == "genres")
        {
            for (unsigned int j = i + 9; j < input.size(); j++)
            {
                if (input[j] != ']')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.genres = parse_list(input.substr(i + 9, i + overshoot).substr(0, overshoot));
        }
        elif (input.substr(i, i + 6).substr(0, 6) == "joined")
        {
            for (unsigned int j = i + 8; j < input.size(); j++)
            {
                if (input[j] != ',')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.joined = std::stoi(input.substr(i + 8, i + overshoot).substr(0, overshoot));
        }
        elif (input.substr(i, i + 9).substr(0, 9) == "downloads")
        {
            for (unsigned int j = i + 11; j < input.size(); j++)
            {
                if (input[j] != ',')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            output.downloads = std::stoi(input.substr(i + 11, i + overshoot).substr(0, overshoot));
        }
    }
    return output;
}

Games parse_games(std::string input)
{
    bool toggle = false;
    Games games;
    std::string append;
    string_list temp;
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (toggle)
        {
            append = append + input[i];
        }
        if (input[i] == '}')
        {
            if (append.size() != 0)
            {
                temp.push_back(append);
                append = "";
                toggle = false;
            }
        }
        elif (input[i] == ':')
        {
            toggle = true;
        }
    }
    for (unsigned int i = 0; i < temp.size(); i++)
    {
        games.push_back(parse_game(temp[i]));
    }
    return games;
}

// Query Data
Tags get_tags()
{
    std::string recv_data = download((url + "tags").c_str());
    Tags tags;
    // interpret the JSON data.
    unsigned int overshoot = 0;
    for (unsigned int i = 0; i < recv_data.size(); i++)
    {
        /*
        Check for tags

        We can't assume positions here. If we did and something in the code changed, we could very quickly end up with breakage.
        Instead, we need to iterate through the string and look for the tags
        */
        // Skip processing any chars we have already handled
        if (overshoot != 0)
        {
            overshoot--;
            continue;
        }
        if (recv_data.substr(i, i + 4) == "genres")
        {
            overshoot = 10;
            for (unsigned int j = i + 8; j < recv_data.size(); j++)
            {
                if (recv_data[j] != ']')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            tags.genres = parse_list(recv_data.substr(i + 8, i + overshoot));
        }
        elif (recv_data.substr(i, i + 9).substr(0, 9) == "platforms")
        {
            overshoot = 1;
            for (unsigned int j = i + 11; j < recv_data.size(); j++)
            {
                if (recv_data[j] != ']')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            tags.platforms = parse_list(recv_data.substr(i + 11, i + overshoot).substr(0, overshoot));
        }
        elif (recv_data.substr(i, i + 7).substr(0, 7) == "ratings")
        {
            overshoot = 0;
            for (unsigned int j = i + 9; j < recv_data.size(); j++)
            {
                if (recv_data[j] != ']')
                {
                    overshoot++;
                }
                else
                {
                    break;
                }
            }
            tags.ratings = parse_list(recv_data.substr(i + 9, i + overshoot).substr(0, overshoot));
        }
    }
    return tags;
}

Games search(std::string free_text)
{
    std::string recv_data = download((url + "search/free-text=" + free_text).c_str());
    return parse_games(recv_data);
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
    return parse_games(recv_data);
}

Games search(string_list tags, std::string free_text)
{
    //piggy back off the free text search function to get the data we need
    //essentially this lets the server do the potentially laborious work of searching multiple things
    //for a given substring
    Games games = search(free_text);
    Games output;
    bool skip = false;
    for (unsigned int i = 0; i < games.size(); i++)
    {
        for (unsigned int j = 0; j < games[i].genres.size(); j++)
        {
            for (unsigned int k = 0; k < tags.size(); k++)
            {
                if (tags[k] == games[i].genres[j])
                {
                    output.push_back(games[i]);
                    skip = true;
                    break;
                }
            }
            if (skip)
            {
                break;
            }
        }
        if (skip)
        {
            skip = false;
        }
        else
        {
            // Check platform and rating
            for (unsigned int k = 0; k < tags.size(); k++)
            {
                if ((tags[k] == games[i].platform) || (tags[k] == games[i].rating))
                {
                    output.push_back(games[i]);
                    break;
                }
            }
        }
    }
    return output;
}

// Download_Info get_download_info(std::string pkg_name)
// {
//
// }
//
Game get_game(std::string pkg_name)
{
    std::string download_url = url + "/games/" + pkg_name;
    std::string recv_data = download(download_url.c_str());
    return parse_game(recv_data);
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
