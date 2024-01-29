# Web Crawler for C
Pareses the webpage of a given URL, extracts any embedded URLs, then retrieves each page recursively, but limits its exploration to a given depth.

# Directions:
Make sure to have libcurl library installed.
-     sudo apt install libcurl4-gnutls-dev

Create a folder that will store all the web pages extracted by the crawler

Makefile is already given. Therefore, type make to compile
-      make

To execute
-      ./crawler url folder_name depth_number
