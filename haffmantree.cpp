#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>

using namespace std;

struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& frequencyMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;
    for (auto& pair : frequencyMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        HuffmanNode* newNode = new HuffmanNode('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }
    return minHeap.top();
}

void generateCode(HuffmanNode* root, const string &prefix, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) return;

    if (!root->left && !root->right) {
        huffmanCode[root->data] = prefix;
    }
    generateCode(root->left, prefix + '0', huffmanCode);
    generateCode(root->right, prefix + '1', huffmanCode);
}

void deleteHuffmanTree(HuffmanNode* node) {
    if (node == nullptr) return;
    deleteHuffmanTree(node->left);
    deleteHuffmanTree(node->right);
    delete node;
}

string encryptString(string &text, unordered_map<char, string>& huffmanCode) {
    string encodeText;
    for (char ch : text) {
        encodeText += huffmanCode.at(ch);
    }
    return encodeText;
}

string decryptstring(string& encodeText, HuffmanNode* root) {
    string decodeText;
    HuffmanNode* current = root;
    for (char bit : encodeText) {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right) {
            decodeText += current->data;
            current = root;
        }
    }
    return decodeText;
}

void displayMenu() {
    cout << "\n===== 哈夫曼树密码加密与解密系统 =====\n";
    cout << "1. 加密字符串\n";
    cout << "2. 解密字符串\n";
    cout << "3. 退出\n";
    cout << "请选择操作: ";
}

int main() {
    int choice;
    string input, encryptedText, decryptedText;
    HuffmanNode* root = nullptr;
    unordered_map<char, string> huffmanCode;
    unordered_map<char, int> frequencyMap;
    ofstream outFile; // 将文件流对象声明在这里

    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // 忽略换行符

        switch (choice) {
        case 1:
            cout << "请输入要加密的字符串: ";
            getline(cin, input);

            // 统计字符频率
            frequencyMap.clear();
            for (char ch : input) {
                frequencyMap[ch]++;
            }

            // 构建哈夫曼树
            if (root) deleteHuffmanTree(root); // 删除旧的哈夫曼树
            root = buildHuffmanTree(frequencyMap);

            // 生成哈夫曼编码表
            huffmanCode.clear();
            generateCode(root, "", huffmanCode);

            // 显示编码结果
            cout << "\n哈夫曼编码表:\n";
            for (const auto& pair : huffmanCode) {
                cout << pair.first << ": " << pair.second << "\n";
            }

            // 加密
            encryptedText = encryptString(input, huffmanCode);
            cout << "\n加密后的文本: " << encryptedText << "\n";

            // 保存加密后的文本到文件
            outFile.open("encrypted.txt");
            if (outFile.is_open()) {
                outFile << encryptedText;
                outFile.close();
                cout << "加密后的文本已保存到 encrypted.txt\n";
            } else {
                cout << "无法打开文件 encrypted.txt 进行写入。\n";
            }
            break;

        case 2:
            if (root == nullptr) {
                cout << "请先进行一次加密操作以构建哈夫曼树。\n";
                break;
            }
            cout << "请输入要解密的哈夫曼编码: ";
            getline(cin, encryptedText);

            // 解密
            decryptedText = decryptstring(encryptedText, root);
            cout << "\n解密后的文本: " << decryptedText << "\n";

            // 保存解密后的文本到文件
            outFile.open("decrypted.txt");
            if (outFile.is_open()) {
                outFile << decryptedText;
                outFile.close();
                cout << "解密后的文本已保存到 decrypted.txt\n";
            } else {
                cout << "无法打开文件 decrypted.txt 进行写入。\n";
            }
            break;

        case 3:
            cout << "感谢使用哈夫曼树密码加密与解密系统！再见！\n";
            if (root) deleteHuffmanTree(root);
            return 0;

        default:
            cout << "无效的选择，请重新选择。\n";
        }
    }

    return 0;
}



