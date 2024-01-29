#include "pagedir.h"

bool pagedir_init(const char *pageDirectory) {
	// Write .crawler into pageDirectory
	char filename[1000];
	sprintf(filename, "%s/.crawler", pageDirectory);
	FILE* fp = fopen(filename, "w");
	
	if (access("./crawler", F_OK) == 1) {
	    fclose(fp);
	    return 0;
	}
	
	return 1;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	// Save page in pageDirectory
	char filename[1000];
	sprintf(filename, "%s/%d", pageDirectory, documentID);
	FILE* fp = fopen(filename, "w");
	
	if (!fp) {
	    fprintf(stderr, "Error. Cannot write to file %s.\n", filename);
	    exit(1);
	}

	fprintf(fp, "%s\n", page->url);
	fprintf(fp, "%d\n", page->depth);
	fprintf(fp, "%s\n", page->html);
	fclose(fp);
}
