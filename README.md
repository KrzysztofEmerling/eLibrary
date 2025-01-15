# eLibrary

eLibrary is a graphical interface application for a library management system. It was developed as a semester project and is written in C with GTK4 for the graphical user interface (GUI). The application stores and manages library data using linked lists. For persistence, the lists are saved in a binary file.

---

## Features

- User-friendly graphical interface powered by GTK4.
- Core functionality for managing library data:
  - Adding books.
  - Sorting books by various criteria (e.g., title, author).
  - Viewing details of books.
  - Managing users in the admin panel.
- Data storage and manipulation implemented using linked lists.

---

## Prerequisites

To build and run eLibrary on a Linux system, ensure the following dependencies are installed:

- **GTK4 development libraries**:
  ```bash
  sudo apt install libgtk-4-dev
  sudo apt install libssl-dev
  ```
  
## Installation

1.Clone the repository.
2.Run the build script:
  ```bash
  ./build.sh
  ```
3.Execute the application:
  ```bash
  ./eLibrary
  ```
