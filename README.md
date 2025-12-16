# Browser Tabs Manager (C)

## Overview
This project implements a simplified browser tabs manager written in C. It simulates core browser functionality such as opening and closing tabs, navigating between tabs, and managing page history using stacks.

The project was developed as part of a university assignment and focuses on data structures, memory management, and clean modular design in C.

---

## Features
- Create and close browser tabs
- Switch between tabs (next / previous / open by ID)
- Open pages inside tabs
- Navigation history support:
  - Backward navigation
  - Forward navigation
- Multiple data structures working together:
  - Doubly linked list for tab management
  - Stacks for page navigation history

---

## Data Structures Used
- **Doubly Linked List** – stores all open tabs
- **Stack** – maintains backward and forward navigation history per tab
- **Custom structs** for browser, tab, page, and stack abstraction

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

---

## Run
The program reads commands from standard input.

```bash
./browser < browser.in
```

## Input Format & Command Guide

### General Format
1. Number of available pages  
2. Page definitions (ID, URL, description)  
3. Number of commands  
4. One command per line  

---

### Supported Commands

- **`NEW_TAB`**  
  Opens a new browser tab and switches to it.

- **`CLOSE`**  
  Closes the current tab.  
  The default tab (ID 0) cannot be closed.

- **`OPEN <tab_id>`**  
  Switches to the tab with the given ID.

- **`NEXT`**  
  Switches to the next tab in the list.

- **`PREV`**  
  Switches to the previous tab in the list.

- **`PAGE <page_id>`**  
  Opens the page with the given ID in the current tab.  
  Clears the forward navigation history.

- **`BACKWARD`**  
  Navigates backward in the page history of the current tab.

- **`FORWARD`**  
  Navigates forward in the page history of the current tab.

- **`PRINT`**  
  Prints all open tab IDs and the description of the current page.

- **`PRINT_HISTORY <tab_id>`**  
  Prints the browsing history (forward, current, backward) of the specified tab.

---

## Notes
- This project emphasizes correct memory allocation and deallocation.
- All dynamically allocated memory is properly released when closing tabs or exiting the program.
- The implementation prioritizes clarity and correctness over UI or real networking.

---

## Technologies & Concepts
- C (ANSI C)
- Pointers and dynamic memory allocation
- Stacks and linked lists
- Modular program design
- Command-driven program flow
