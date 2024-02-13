# What is This Project?

This is a repository for managing CLI tools created for my personal use.

## chscreen

In a multi-display environment, moving the cursor across displays can be tiresome, wearing out both your fingers and the trackpad. `chscreen` is a CLI tool developed to save my fingers and trackpad, allowing the cursor to move to any display instantly.

### Build

A Makefile has not been created. Please compile directly using the following command:

```
clang -O2 -framework ApplicationServices ./main.c -o chscreen
```

### How to Use

It's recommended to register it as a shortcut key and launch it via Automator.