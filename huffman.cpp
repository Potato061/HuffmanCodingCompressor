

// i dont like #include <bits/stdc++.h> :D

#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iomanip>

struct HNode {
    char ch;
    int freq;
    HNode* left;
    HNode* right;
    HNode(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
    HNode(char ch, int freq, HNode* left, HNode* right) : ch(ch), freq(freq), left(left), right(right) {}
};

struct compare {
    bool operator()(HNode* l, HNode* r) {
        return l->freq > r->freq;
    }
};

void printCode(HNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (root == nullptr) return;
    
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    
    printCode(root->left, str + "0", huffmanCode);
    printCode(root->right, str + "1", huffmanCode);
}

std::vector<unsigned char> packBitsToBytes(const std::string& encoded) {
    std::vector<unsigned char> result;
    unsigned char currentByte = 0;
    int bitCount = 0;
    
    for (char c : encoded) {
        currentByte = (currentByte << 1);
        if (c == '1') currentByte |= 1;
        bitCount++;
        
        if (bitCount == 8) {
            result.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }
    }
    
    if (bitCount > 0) {
        currentByte = currentByte << (8 - bitCount);
        result.push_back(currentByte);
    }
    
    return result;
}

void saveHuffmanCodes(const std::string& filename, const std::unordered_map<char, std::string>& huffmanCode) {
    std::ofstream file(filename);
    for (auto pair : huffmanCode) {
        file << static_cast<int>(pair.first) << " " << pair.second << "\n";
    }
    file.close();
}

void saveCompressedToFile(const std::string& filename, const std::vector<unsigned char>& compressedData) {
    std::ofstream file(filename, std::ios::binary);
    for (unsigned char byte : compressedData) {
        file.put(byte);
    }
    file.close();
}

void compressFile(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Cannot open input file " << inputFileName << std::endl;
        return;
    }
    
    inputFile.seekg(0, std::ios::end);
    size_t originalSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    
    std::string text;
    char ch;
    while (inputFile.get(ch)) {
        text += ch;
    }
    inputFile.close();

    std::unordered_map<char, int> freq;
    for (char c : text) {  
        freq[c]++;
    }

    std::priority_queue<HNode*, std::vector<HNode*>, compare> pq;
    for (auto pair : freq) {
        pq.push(new HNode(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        HNode* left = pq.top(); pq.pop();
        HNode* right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(new HNode('\0', sum, left, right));
    }

    HNode* root = pq.top();

    std::unordered_map<char, std::string> huffmanCode;
    printCode(root, "", huffmanCode);

    std::string encodedStr = "";
    for (char ch : text) {
        encodedStr += huffmanCode[ch];
    }

    std::vector<unsigned char> compressedBytes = packBitsToBytes(encodedStr);
    saveCompressedToFile(outputFileName, compressedBytes);
    
    std::string codeFileName = outputFileName + ".codes";
    saveHuffmanCodes(codeFileName, huffmanCode);

    size_t compressedSize = compressedBytes.size();
    double ratio = (1.0 - (double)compressedSize / originalSize) * 100.0;
    
    std::cout << "=== Compression Results ===" << std::endl;
    std::cout << "Original size:  " << originalSize << " bytes" << std::endl;
    std::cout << "Compressed size: " << compressedSize << " bytes" << std::endl;
    std::cout << "Compression ratio: " << std::fixed << std::setprecision(1) << ratio << "%" << std::endl;
    std::cout << "Space saved: " << (originalSize - compressedSize) << " bytes" << std::endl;
    std::cout << "Codes saved to: " << codeFileName << std::endl;
}


void decompressFile(const std::string& compressedFile, const std::string& outputFile) {
    // 1. Read the Huffman codes from the .codes file
    std::ifstream codeFile(compressedFile + ".codes");
    std::unordered_map<std::string, char> codeToChar; // Reverse mapping: code -> character
    
    std::string line;
    while (std::getline(codeFile, line)) {
        if (line.empty()) continue;
        
        // Parse: "65 1010" -> char=65, code="1010"
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            int charValue = std::stoi(line.substr(0, spacePos));
            std::string code = line.substr(spacePos + 1);
            codeToChar[code] = static_cast<char>(charValue);
        }
    }
    codeFile.close();

    // 2. Read the compressed binary file
    std::ifstream compFile(compressedFile, std::ios::binary);
    std::vector<unsigned char> compressedBytes;
    unsigned char byte;
    
    while (compFile.read(reinterpret_cast<char*>(&byte), 1)) {
        compressedBytes.push_back(byte);
    }
    compFile.close();

    // 3. Convert bytes back to bit string
    std::string bitString = "";
    for (unsigned char b : compressedBytes) {
        for (int i = 7; i >= 0; i--) {
            if (b & (1 << i)) {
                bitString += '1';
            } else {
                bitString += '0';
            }
        }
    }

    // 4. Decoding using Huffman codes
    std::string currentCode = "";
    std::string decodedText = "";
    
    for (char bit : bitString) {
        currentCode += bit;
        if (codeToChar.find(currentCode) != codeToChar.end()) {
            decodedText += codeToChar[currentCode];
            currentCode = ""; // Reset for the next code
        }
    }

    // 5.Saving decompressed text
    std::ofstream outFile(outputFile);
    outFile << decodedText;
    outFile.close();
    
    std::cout << "File decompressed to: " << outputFile << std::endl;
}

int main() {
    std::string choice;
    
    std::cout << "Choose: (1) Compress text (2) Decompress file" << std::endl;
    std::getline(std::cin, choice);

    if (choice == "1") {
        // The multi-line input for compression...
        std::string content;
        std::string line;
        
        std::cout << "Enter your text (type 'END' on ITS OWN LINE to finish :D)\n";
        while (true) {
            std::getline(std::cin, line);
            if (line == "END") break;
            content += line + "\n";
        }
        
        std::ofstream temp("input.txt");
        temp << content;
        temp.close();
        
        compressFile("input.txt", "output.bin");
        std::cout << "Use option 2 to decompress and get your text back!\n";
        
        // decompression ...
    } else if (choice == "2") {
        decompressFile("output.bin", "decompressed.txt");
        std::cout << "Your original text is in 'decompressed.txt'!\n";
    } else {
        std::cout << "Invalid choice!\n";
    }
    
    return 0;
}