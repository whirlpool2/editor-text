# editor-text
Simple Text Editor (C++ & SFML)

A functional, GUI-based text editor built from scratch as a first-semester foundational project for the ["Introduction to Programming"](https://edu.info.uaic.ro/introducere-programare/) course at [FII (UAIC)](https://info.uaic.ro/).

# Collaboration & Teamwork

Developed in a 3-person team ([Mihail Bereșteanu](https://github.com/Mihai0929), [Ștefan Mihalache](https://github.com/whirlpool2), [Lucas Pascaniuc](https://github.com/lucasjoaca)), focusing on collaborative problem-solving and shared version control.

# Technical Features

Despite being an early-career project, it implements several non-trivial editor mechanics:

- Data Structure: Utilizes a doubly-linked list to manage text lines, allowing for efficient insertion and deletion.
- GUI & Rendering: Built using the SFML 2.6.2 library for hardware-accelerated text rendering and UI components.
- Core Functionality: Supports vertical/horizontal navigation, mouse-click cursor positioning, visual scrolling, and text selection.
- File I/O: Supports loading and saving files in ANSI/ASCII encoding.

# ⚠️ Retrospective & Limitations

- Performance: The current rendering implementation is optimized for files under 20,000 characters; performance scaling for larger files is a known bottleneck.
- Repository Structure: The repository intentionally includes the full SFML library to ensure "out-of-the-box" compatibility for university evaluations, a practice we have since replaced with proper dependency management.
- Code Quality: As a first-semester project, the codebase reflects an early understanding of OOP and manual memory management.
