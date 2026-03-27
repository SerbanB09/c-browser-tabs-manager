# Browser Tabs Manager (C)

A simplified browser tabs manager written in C that simulates core browser functionality: opening and closing tabs, navigating between tabs, and managing page history using stacks.

---

## Features

- Create and close browser tabs
- Switch between tabs (next / previous / open by ID)
- Open pages inside tabs
- Navigation history per tab:
  - Backward navigation
  - Forward navigation

---

## Data Structures Used

| Structure | Role |
|---|---|
| **Doubly Linked List** | Stores and manages all open tabs |
| **Stack** | Maintains backward and forward navigation history per tab |
| **Custom structs** | Abstractions for browser, tab, page, and stack |

---

## Project Structure

```
.
├── browser.c
├── browser.in
├── Makefile
└── README.md
```

---

## Build

```bash
make
```

## Run

The program reads commands from standard input:

```bash
./browser < browser.in
```

---

## Input Format

```
<number of available pages>
<page definitions: ID, URL, description>
<number of commands>
<one command per line>
```

### Supported Commands

| Command | Description |
|---|---|
| `NEW_TAB` | Opens a new tab and switches to it |
| `CLOSE` | Closes the current tab (default tab ID 0 cannot be closed) |
| `OPEN <tab_id>` | Switches to the tab with the given ID |
| `NEXT` | Switches to the next tab in the list |
| `PREV` | Switches to the previous tab in the list |
| `PAGE <page_id>` | Opens a page in the current tab; clears forward history |
| `BACKWARD` | Navigates backward in the current tab's history |
| `FORWARD` | Navigates forward in the current tab's history |
| `PRINT` | Prints all open tab IDs and the current page description |
| `PRINT_HISTORY <tab_id>` | Prints the full browsing history of the specified tab |

---

## Notes

- All dynamically allocated memory is properly released when closing tabs or exiting.
- The implementation prioritizes correctness and clarity over UI or real networking.

---

## Technologies & Concepts

- **Language:** C (ANSI C)
- **Concepts:** Pointers, dynamic memory allocation, stacks, doubly linked lists, modular design, command-driven program flow
