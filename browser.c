/* BIBIRE Serban-Gabriel - 312CB */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure for a web page
typedef struct page {
	int id;
	char url[50];
	char *description;
} TPage;

// Stack node used for page navigation history
struct stack {
    struct page *data;
    struct stack *next;
};

// Structure for a browser tab
typedef struct tab {
	int id;
	struct page *currentPage;
	struct stack *backwardStack;
	struct stack *forwardStack;
} TTab;

// Structure for the tabs list (doubly linked list)
struct tabsList {
    TTab *info;
	struct tabsList *next;
	struct tabsList *prev;
};

// Structure for the browser (contains the tabs list)
typedef struct browser {
    struct tab *current;
    struct tabsList list;
} TBrowser;

// Browser initialization function and default tab creation
void startBrowser(TBrowser *browser, TPage *defaultPage) {
	// Initialize the browser tabs list (doubly linked list)
	browser->list.info = NULL;
	browser->list.next = &(browser->list);
	browser->list.prev = &(browser->list);

	// Create initial tab
	TTab* startingTab = malloc(sizeof(TTab));
	startingTab->id = 0; // The initial tab has ID 0
	startingTab->currentPage = defaultPage; // Current page is the default page
    startingTab->backwardStack = NULL; // Stack for backward navigation
    startingTab->forwardStack = NULL; // Stack for forward navigation

	// Add the tab to the tabs list
	struct tabsList *newCell = malloc(sizeof(struct tabsList));
	newCell->info = startingTab;
	newCell->next = &(browser->list);
	newCell->prev = &(browser->list);
	browser->list.next = newCell;
	browser->list.prev = newCell;
	browser->current = startingTab; // Set the current tab to the initial tab
}

// Function to add a new tab to the list
void addTabList(struct tabsList *list, TTab *newTab)
{
	struct tabsList *newCell = malloc(sizeof(struct tabsList));
	newCell->info = newTab;
	newCell->next = list;
	newCell->prev = list->prev;
	list->prev->next = newCell;
	list->prev = newCell;
}

// Function to create a new tab
void new_tab(TBrowser* browser, TPage* defaultPage, int *tabCnt)
{
	(*tabCnt)++;
	TTab* newTab = malloc(sizeof(TTab));
	newTab->id = *tabCnt; // Tab ID is incremented
	newTab->currentPage = defaultPage;
	newTab->backwardStack = NULL;
	newTab->forwardStack = NULL;

	// Add the tab to the list
	addTabList(&(browser->list), newTab);
	browser->current = newTab; // Set the current tab to the newly created one
}

// Function to print all tabs in the browser
void printTabs(TBrowser *browser, FILE* out)
{
    struct tabsList *current = browser->list.next;

	// Find the current tab in the list
	while (current->info != browser->current) {
		current = current->next;
	}
    
	// Traverse all tabs and print them
    do {
        fprintf(out, "%d ", current->info->id);
		if (current->next->info != NULL) {
        	current = current->next;
		} else {
			current = current->next->next;
		}
    } while (current->info != browser->current);

    fprintf(out, "\n%s\n", browser->current->currentPage->description);
}

// Function to free stack memory
void free_stack(struct stack *s)
{
    while (s) {
        struct stack *tmp = s;
        s = s->next;
        free(tmp);
    }
}

// Function to close a tab
void close_tab(TBrowser *browser, FILE* out)
{
	if (browser->current->id == 0) {
		// The default tab cannot be closed
		fprintf(out, "403 Forbidden\n");
		return;
	}
	struct tabsList *current = browser->list.next, *prev = NULL, *next = NULL;
	while (current != &(browser->list)) {
		if (current->info == browser->current) {
			prev = current->prev;
			next = current->next;
			break;
		}
		current = current->next;
	}
	// Update the tabs list
	prev->next = next;
	next->prev = prev;
	free_stack(current->info->backwardStack);
	free_stack(current->info->forwardStack);
	free(current->info);
	free(current);
	// Set the current tab to the previous one
	browser->current = prev->info;
}

// Function to open a tab by ID
void open_tab(TBrowser *browser, int id, FILE* out)
{
	struct tabsList *current = browser->list.next;
	while (current != &(browser->list)) {
		if (current->info->id == id) {
			browser->current = current->info;
			return;
		}
		current = current->next;
	}
	// If the tab does not exist, print an error message
	fprintf(out, "403 Forbidden\n");
}

// Tab navigation functions
void next_tab(TBrowser *browser)
{
	struct tabsList *current = browser->list.next;
	while (current->info != browser->current) {
		current = current->next;
	}
	if (current->next == &(browser->list)) {
		current = current->next->next;
	} else {
		current = current->next;
	}
	browser->current = current->info;
}

void prev_tab(TBrowser *browser)
{
	struct tabsList *current = browser->list.next;
	while (current->info != browser->current) {
		current = current->next;
	}
	if (current->prev == &(browser->list)) {
		current = current->prev->prev;
	} else {
		current = current->prev;
	}
	browser->current = current->info;
}

// Function to open a page in the current tab
void open_page(TBrowser *browser, int val, FILE* out, int nrPages, TPage *pages)
{
	TPage *newPage = NULL;
	for (int i = 0; i < nrPages; i++) {
        if (pages[i].id == val) {
            newPage = &pages[i];
            break;
        }
    }

	if (newPage == NULL) {
        fprintf(out, "403 Forbidden\n");
        return;
    }
	// Save the current page in the backward stack
	struct stack *lastPage = malloc(sizeof(struct stack));
	lastPage->data = browser->current->currentPage;
	lastPage->next = browser->current->backwardStack;
	browser->current->backwardStack = lastPage;

	// Clear the forward stack
	while (browser->current->forwardStack) {
		struct stack *temp = browser->current->forwardStack;
		browser->current->forwardStack = temp->next;
		free(temp);
	}

	browser->current->currentPage = newPage;
}

// Function to print a tab's history
void print_history(TBrowser *browser, int idTab, FILE *out)
{
	struct tabsList *current = browser->list.next;
    while (current != &(browser->list)) {
        if (current->info->id == idTab) {
            break;
        }
        current = current->next;
    }
	TTab *tab = current->info;

	if (current == &browser->list) {
        fprintf(out, "403 Forbidden\n");
        return;
    }

	// Print pages from the forward stack
	struct stack *forward = tab->forwardStack;
    struct stack *temp = NULL;
    while (forward) {
        struct stack *curr = malloc(sizeof(struct stack));
        curr->data = forward->data;
        curr->next = temp;
        temp = curr;
        forward = forward->next;
    }

	struct stack *toPrintF = temp;
	while (toPrintF) {
		fprintf(out, "%s\n", toPrintF->data->url);
		struct stack *aux = toPrintF;
		toPrintF = toPrintF->next;
		free(aux);
	}

	// Print the current page
	if (tab->currentPage) {
        fprintf(out, "%s\n", tab->currentPage->url);
	}

	// Print pages from the backward stack
	struct stack *toPrintB = tab->backwardStack;
	while (toPrintB) {
		fprintf(out, "%s\n", toPrintB->data->url);
		toPrintB = toPrintB->next;
	}
}

// Function to navigate backward in page history
void go_backward (TBrowser *browser, FILE* out)
{
	struct tab *currentTab = browser->current;

	if (!currentTab->backwardStack) {
		fprintf(out, "403 Forbidden\n");
		return;
	}

	// Move the current page to the forward stack
	struct stack *new = malloc(sizeof(struct stack));
	new->data = currentTab->currentPage;
	new->next = currentTab->forwardStack;
	currentTab->forwardStack = new;

	// Move to the page from the backward stack
	currentTab->currentPage = currentTab->backwardStack->data;
	struct stack *temp = currentTab->backwardStack;
	currentTab->backwardStack = currentTab->backwardStack->next;
	free(temp);
}

// Function to navigate forward in page history
void go_forward(TBrowser *browser, FILE* out)
{
    TTab *currentTab = browser->current;

    if (!currentTab->forwardStack) {
        fprintf(out, "403 Forbidden\n");
        return;
    }

    // Move the current page to the backward stack
    struct stack *new = malloc(sizeof(struct stack));
    new->data = currentTab->currentPage;
    new->next = currentTab->backwardStack;
    currentTab->backwardStack = new;

    // Move to the page from the forward stack
    struct stack *temp = currentTab->forwardStack;
    currentTab->currentPage = temp->data;
    currentTab->forwardStack = temp->next;
    free(temp);
}

// Function to free browser memory
void freeBrowser(TBrowser *browser)
{
    struct tabsList *sentinel = &browser->list;
    struct tabsList *current = sentinel->next;

    // Traverse the tabs list and free memory
    while (current != sentinel) {
        struct tabsList *next = current->next;

        struct stack *s = current->info->backwardStack;
        while (s) {
            struct stack *tmp = s;
            s = s->next;
            free(tmp);
        }

        s = current->info->forwardStack;
        while (s) {
            struct stack *tmp = s;
            s = s->next;
            free(tmp);
        }

        free(current->info);
        free(current);

        current = next;
    }

    sentinel->next = sentinel;
    sentinel->prev = sentinel;
}

int main() {
	// Initialize variables
	int tabCnt = 0;
	int nrPages = 0, nrOp = 0;

	// Open input and output files
	FILE *in = fopen("browser.in", "rt");
	FILE *out = fopen("browser.out", "wt");

	// Initialize the browser and the default tab
	TBrowser browser;
	TPage defaultPage = {0, "https://acs.pub.ro/", strdup("Computer Science")};
	startBrowser(&browser, &defaultPage);

	// Read pages data from file
	fscanf(in, "%d", &nrPages);
    TPage *pages = malloc(nrPages * sizeof(TPage));

	for (int i = 0; i < nrPages; i++) {
		char buffer[512];

    	// Read page ID
    	fscanf(in, "%d\n", &pages[i].id);

		// Read page URL
		fgets(pages[i].url, sizeof(pages[i].url), in);
		pages[i].url[strcspn(pages[i].url, "\n")] = '\0';

		// Read page description
		fgets(buffer, sizeof(buffer), in);
		buffer[strcspn(buffer, "\n")] = '\0';
		pages[i].description = strdup(buffer);
	}
	
	// Read operations and execute commands
	fscanf(in, "%d\n", &nrOp);
	for (int i = 0; i < nrOp; i++) {
		char command[200];
		fgets(command, sizeof(command), in);
		command[strcspn(command, "\n\r")] = '\0';
		char onlyCommand[100];
		int val = 0;
		sscanf(command, "%s %d", onlyCommand, &val);
		if (strcmp(command, "NEW_TAB") == 0) {
			new_tab(&browser, &defaultPage, &tabCnt);
		} else if (strcmp(command, "CLOSE") == 0) {
			close_tab(&browser, out);
		} else if (strcmp(command, "PRINT") == 0) {
			printTabs(&browser, out);
		} else if (strcmp(onlyCommand, "OPEN") == 0) {
			open_tab(&browser, val, out);
		} else if (strcmp(command, "NEXT") == 0) {
			next_tab(&browser);
		} else if (strcmp(command, "PREV") == 0) {
			prev_tab(&browser);
		} else if (strcmp(onlyCommand, "PAGE") == 0) {
			open_page(&browser, val, out, nrPages, pages);
		} else if (strcmp(onlyCommand, "PRINT_HISTORY") == 0) {
			print_history(&browser, val, out);
		} else if (strcmp(command, "BACKWARD") == 0) {
			go_backward(&browser, out);
		} else if (strcmp(command, "FORWARD") == 0) {
			go_forward(&browser, out);
		}
	}

	// Free memory
	free(defaultPage.description);
	for (int i = 0; i < nrPages; i++) {
    	free(pages[i].description);
	}
	free(pages);

	// Free browser memory
	freeBrowser(&browser);

	// Close files
	fclose(in);
	fclose(out);

	return 0;
}
