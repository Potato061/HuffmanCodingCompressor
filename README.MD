# Huffman File Compressor

A C++ implementation of Huffman coding for text compression and decompression with multi-line input support.

## 📋 Description
This project implements lossless data compression using Huffman coding. It can compress text input and decompress it back to the original form, demonstrating fundamental compression algorithms.

## ✨ Features
- **Text Compression**: Compresses user-input text using Huffman coding
- **Multi-line Input**: Support for paragraphs and multi-line text
- **Compression Analytics**: Shows compression ratio and space saved
- **Lossless Decompression**: Perfect reconstruction of original text
- **Binary File Output**: Efficient compressed binary format

## 🚀 Quick Start

### Prerequisites
- **Compiler**: GCC (MinGW) on Windows
- **IDE**: VS Code with C++ extension (optional)

### Compilation & Execution

#### Method 1: Using VS Code (Recommended)
1. Open the project in VS Code
2. Make sure `huffman.cpp` is the active file
3. Press **`Ctrl+Shift+B`** to build the project
4. Press **`F5`** to run the program

#### Method 2: Using Terminal/Command Prompt
1. Open terminal in your project directory
2. Compile the code:
   ```bash
   g++ -fdiagnostics-color=always -g huffman.cpp -o huffman.exe
3.Then write "./huffman.exe" to run the program

### Project Structure:
    huffman-compressor/
    ├── .vscode/
    │ └── tasks.json # VS Code build configuration
    ├── huffman.cpp # Main source code
    ├── README.md # This file
    └── (Generated files during execution)
