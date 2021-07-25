# VAPI
The Vetala API, VAPI (Va-pee) for short.

## About
VAPI is an easy-to-use API for C++, Python, and Rust that allows developers to quickly and easily get information about a Vetala system, change or obtain certain settings, or interact with the [Vetala Store API](https://github.com/drauger-os-development/vetala-store-backend).


## Obtaining Info
VAPI can obtain info on the following things:

 * CPU governor

 * Installation Native Status

    * This depends on whether the CPU in use fits a certain set of criteria.


 * Installed packages and other apps

    * packages can be installed from:
       * `apt`
       * `snap`
       * `flatpak`
       * Or external sources from the Vetala Store

## Requesting Changes
VAPI can request certain changes to be made to the system. Some of these changes may require user approval.

 * CPU governor

 * Dependency download

   * Since `apt` resolves dependencies automatically, and `snap` and `flatpak` packages come with all dependencies built-in, this should be limited in use to apps from external sources on the Vetala Store, requesting packages be installed in order to set themselves up or run properly.

   *  This requires user approval.

## Interacting with the Vetala Store API
This library provides an easy, safe, and reliable way to query the Vetala Store API. It also provides some functions the Vetala Store API does not.

 * Searching

   * Searches can be performed with a simple `vstore_search()` request, be it with tags or free text.

   * Searches can also be performed on both tags and free text simultaneously.

 * Tags

   * Calling `vstore_tags()` will return a list of valid tags to choose from.


 * Downloading

   * When downloading an app from an external source, `vstore_install_external_URL()` can be called, with the URL passed as an argument in order to have VAPI handle installation for you.

   * If download begins and no URL is available, `vstore_install_external_local()` can be called, with the location of the fully downloaded TAR ball passed as an argument. This will have VAPI handle the installation procedure for you.
