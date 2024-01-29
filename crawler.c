#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "crawler.h"
#include "curl.h"
#include "pagedir.h"
#include "url.h"

/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
	// Fill in with your implementation
	if (argc != 4) {
	    fprintf(stderr, "Error. Incorrect number of arguments\n");
	    fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth\n", argv[0]);
	    exit(1);
	}
	
	*seedURL = argv[1];
	*pageDirectory = argv[2];
	
	for (unsigned int i = 0; i < strlen(argv[3]); i++) {
	    if (!isdigit(argv[3][i])) {
	        fprintf(stderr, "Error. maxDepth should be integer.\n");
	        exit(1);
	    }
	}
	*maxDepth = atoi(argv[3]);
	
	if (*maxDepth < 0 || *maxDepth > 10) {
	    fprintf(stderr, "Error. maxDepth should be in range [0, 10].\n");
	    exit(1);
	}
}

static void addPageToBag(bag_t *pagesToCrawl, webpage_t* page) {
	pagesToCrawl->size = pagesToCrawl->size + 1;

    	if (pagesToCrawl->size == 1) {
        	pagesToCrawl->pages = malloc(sizeof(webpage_t*));
    	}
    	else {
        	pagesToCrawl->pages = realloc(pagesToCrawl->pages,
                          sizeof(webpage_t*) * pagesToCrawl->size);
   	}
    	pagesToCrawl->pages[pagesToCrawl->size - 1] = page;
}


/**
 * Scans a webpage for URLs.
 */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) {

	page->html = download(page->url, &page->length);
	
	if (!page->html) {
	    return;
	}
	
	printf("%d\tFetched: %s\n", page->depth, page->url);
	printf("%d\tScanning: %s\n", page->depth, page->url);
	
	char *ptr_href = strstr(page->html, "<a href=");
	
	while (ptr_href) {
	    while (*ptr_href != '"') {
	        // Keep going until we reach "
	        ptr_href++;
	    }
	    // Increment one more time to reach url
	    ptr_href++;
	    
	    // Save url until we reach end of url "
	    int i = 0;
            char url[1000];
            while(*ptr_href != '"') {
                url[i] = *ptr_href;
                i++;
                ptr_href++;
            }
            url[i] = '\0';
            
            // Check if url is internal and add to bag if yes
            char* normalizedURL = normalizeURL(page->url, url);

            if (normalizedURL) {
                printf("%d\tFound: %s\n", page->depth, normalizedURL);
                if (isInternalURL(page->url, normalizedURL)) {
                    webpage_t* subPage = malloc(sizeof(webpage_t));
		    		subPage->url = strdup(normalizedURL);
		    		subPage->depth = page->depth + 1;
	            	subPage->html = NULL;
	       	    	subPage->length = 0;
	            	printf("%d\tAdded: %s\n", page->depth, normalizedURL);
		    		addPageToBag(pagesToCrawl, subPage);  
                }
               
            }
            free(normalizedURL);
         
            // Move on to next <a href=
            ptr_href = strstr(ptr_href, "<a href=");
	}
}


/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {
    // Create hashtable for seen pages
    hashtable_t* pagesSeen = hashtable_new(10);
    
    bag_t *pagesToCrawl = malloc(sizeof(bag_t));
    pagesToCrawl->size = 0;
    
    webpage_t* seedPage = malloc(sizeof(webpage_t));
    seedPage->url = strdup(seedURL);
    seedPage->depth = 0;
    seedPage->html = NULL;
    seedPage->length = 0;
	int current_depth = 0;
    
    addPageToBag(pagesToCrawl, seedPage);
    
    int documentID = 1;
    while (true) {
        bool newPagesFound = false;
		bool maxDepthReached = false;
        
        // Create a temporary array to store newly discovered pages
        bag_t* newPages = malloc(sizeof(bag_t));
        newPages->size = 0;

        for (int i = 0; i < pagesToCrawl->size; i++) {
            webpage_t* page = pagesToCrawl->pages[i];
            sleep(1);

            if (page->depth <= maxDepth) {

				if (strstr(page->url, "#") != NULL) {
					printf("%d\tIgnURL: %s\n", page->depth, page->url);
					continue;
				}
				char *normalURL = normalizeURL(seedURL, page->url);
				if(!hashtable_find(pagesSeen, page->url) && hashtable_insert(pagesSeen, page->url, page) && isInternalURL(seedURL, normalURL)) {
					free(normalURL);
                	pageScan(page, newPages, pagesSeen);
					pagedir_save(page, pageDirectory, documentID);
					documentID += 1;
				}
				else if(hashtable_find(pagesSeen, page->url)) {
					free(normalURL);
					printf("%d\tIgnDupl: %s\n", page->depth, page->url);
				}
				else {
					free(normalURL);
					printf("%d\tIgnExtrn: %s\n", page->depth, page->url);
				}
            } else {
                printf("%d\tMaxDepth Reached: %s\n", page->depth, page->url);
				maxDepthReached = true;
				break;
            }
		

        }

        // Add newly discovered pages to the main bag
        for (int i = 0; i < newPages->size; i++) {
            addPageToBag(pagesToCrawl, newPages->pages[i]);
        }

        free(newPages->pages);
        free(newPages);

		if (maxDepthReached) {
			break;
		}
    }

    // Free pagesSeen and pagesToCrawl
    hashtable_delete(pagesSeen, NULL);
    for (int i = 0; i < pagesToCrawl->size; i++) {
        webpage_t* page = pagesToCrawl->pages[i];
        free(page->url);
        if (page->html) {
            free(page->html);
        }
        free(page);
    }
    free(pagesToCrawl->pages);
    free(pagesToCrawl);
}



int main(const int argc, char *argv[]) {
	// Fill in with your implementation
	char *seedURL;
	char *pageDirectory;
	int maxDepth;
	
	parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
	if (!pagedir_init(pageDirectory)) {
	    fprintf(stderr, "Error. Could not write .crawler file to pageDirectory.\n");
	    exit (1);
	}
	
	crawl(seedURL, pageDirectory, maxDepth);
	return 0;
}



