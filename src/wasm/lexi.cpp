// Copyright 2024 Michael Vaden

//#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

extern "C" {
/*
struct TokenData {
    int tokenType;
    std::string *src;
    int start;
    int end;
};*/

struct Lexeme {
    std::string name;   // name of Lexeme (i.e. "STRING")
    std::string expr;   // expression to match (i.e. "\"[a-zA-Z0-9\\s\\}]*\"")
};

struct TokenNode {
    //std::shared_ptr<TokenData> data;
    int tokenType;
    int end;
    int start;
    TokenNode* next;
    TokenNode* prev;
    std::string *src;
};

//typedef std::shared_ptr<LLNode> TokenNode;

class Tokenizer {
 public:
    Tokenizer() {
        currentRule = 0;

        addRule("CONTENT", "");     // id 0 reserved for content node
    }

    void addRule(std::string name, std::string expr) {
        Lexeme lexeme {name, expr};
        lexemes[currentRule] = lexeme;

        currentRule++;
    }

    TokenNode* tokenize(std::string *src) {
        TokenNode* root = createNode(
            0, src, 0, src->length());
        root->next = nullptr;
        root->prev = nullptr;

        for (int i = 1; i < currentRule; i++) {
            tokenizeNode(root, i);
        }

        return root;
    }

    std::string tokenToString(TokenNode *node, bool displayContent) {
        std::stringstream ss;

        TokenNode *curr = node;

        while (curr != nullptr) {
            ss << tokenNodeToString(curr, displayContent);
            curr = curr->next;
        }

        return ss.str();
    }

    std::unordered_map<int, Lexeme> lexemes;
 private:
    int currentRule;
    
    TokenNode* createNode(int tokenType, std::string *src, int start, int end) {
        TokenNode *node = new TokenNode;

        node->tokenType = tokenType;
        node->end = end;
        node->start = start;
        node->src = src;

        return node;
    }

    std::string singleNodeToString(TokenNode *node) {
        if (node == nullptr) return "<null>";

        std::stringstream ss;

        ss << "<" << lexemes.at(node->tokenType).name << " ";
        ss << node->start << " " << node->end << ">";

        return ss.str();
    }

    std::string tokenNodeToString(TokenNode *node, bool displayContent) {
        if (node == nullptr) return "<null>";

        std::stringstream ss;

        ss << singleNodeToString(node) << " - ";
        ss << singleNodeToString(node->prev) << ", ";
        ss << singleNodeToString(node->next);

        if (displayContent) {
            std::string display = *node->src;
            int i = node->start;
            int j = node->end - node->start;
            ss << "\n" << display.substr(i, j) << "\n";
        }

        return ss.str();
    }

    void replace(TokenNode *replaced, TokenNode *replacement) {
        if (replaced == nullptr) {
            throw std::runtime_error("Replaced node cannot be null.");
        }

        if (replacement == nullptr) {
            throw std::runtime_error("Replacement node cannot be null.");
        }

        // record next node before replacement
        TokenNode *oldNext = replaced->next;

        // link replacement to replaced previous node 
        replacement->prev = replaced->prev;

        // locate last node of replacement
        TokenNode *currNode = replacement;

        while (currNode->next != nullptr) {
            currNode = currNode->next;
        }

        // replace next node of replacement with replaced next
        currNode->next = oldNext;

        if (oldNext != nullptr) {
            oldNext->prev = currNode;
        }

        // finally replace the replacement
        *replaced = *replacement;
    }

    void tokenizeContentNode(TokenNode *node, int tokenType) {
        // regex mapped to token type
        std::string expression = lexemes.at(tokenType).expr;

        int oldEnd = node->end;               // length of src
        std::string *src = node->src;         // should be copied to all nodes

        // src file content for within content node scope
        std::string input = *src;
        int i = node->start;
        int j = node->end - node->start;
        input = input.substr(i, j);

        // match expression
        std::regex regex(expression);
        std::smatch match;
        auto begin = std::sregex_iterator(input.begin(), input.end(), regex);
        auto end = std::sregex_iterator();

        // current index of search
        int offset = node->start;
        int currIndex = 0;
        int matchStart = 0;
        int matchEnd = 0;
        bool matchFound = false;

        TokenNode *rootNode = nullptr;
        TokenNode *currNode = nullptr;

        for (auto it = begin; it != end; ++it) {
            // match information
            matchFound = true;
            match = *it;
            std::string matchContents = match.str(0);
            matchStart = match.position(0);
            matchEnd = matchStart + match.length(0);

            // zero length match should be rejected
            if (matchStart == matchEnd) return;

            // prefix content node
            if (currIndex != matchStart) {
                TokenNode *node = createNode(
                    0, src, currIndex + offset, matchStart + offset);

                node->next = nullptr;
                node->prev = nullptr;

                // update index
                currIndex = matchStart;

                // link node
                if (rootNode == nullptr) {      // first node case
                    rootNode = node;
                    currNode = rootNode;
                } else {                        // not first
                    node->prev = currNode;
                    node->prev->next = node;

                    currNode = node;
                }
            }

            // lexical node
            TokenNode *node = createNode(
                tokenType, src, matchStart + offset, matchEnd + offset);
            node->next = nullptr;
            node->prev = nullptr;

            // update index
            currIndex = matchEnd;

            // link node
            if (rootNode == nullptr) {      // first node case
                rootNode = node;
                currNode = rootNode;
            } else {                        // not first
                node->prev = currNode;
                node->prev->next = node;

                currNode = node;
            }
        }

        // suffix content node
        if (currIndex + offset != oldEnd && matchFound) {
            TokenNode *node = createNode(
                0, src, currIndex + offset, oldEnd);

            node->next = nullptr;
            node->prev = nullptr;

            // link node
            if (rootNode == nullptr) {      // first node case
                rootNode = node;
                currNode = rootNode;
            } else {                        // not first
                node->prev = currNode;
                node->prev->next = node;

                currNode = node;
            }
        }

        // insert new node tree in place of old
        if (rootNode != nullptr) {
            replace(node, rootNode);
        }
    }

    void tokenizeNode(TokenNode *node, int tokenType) {
        TokenNode *currNode = node;

        while (currNode != nullptr) {
            if (currNode->tokenType == 0) {
                tokenizeContentNode(currNode, tokenType);
            }

            currNode = currNode->next;
        }
    }
};

TokenNode* tokenize(Tokenizer *tokenizer, const char* srcPtr) {
    std::string src(srcPtr);

    TokenNode *root = tokenizer->tokenize(&src);

    return root;
}

Tokenizer* initTokenizer() {
    Tokenizer *tokenizer = new Tokenizer();
    
    return tokenizer;
}

void addRule(Tokenizer* tokenizer, const char* name, const char* expr) {
    std::string nameStr(name);
    std::string exprStr(expr);

    tokenizer->addRule(nameStr, exprStr);
}

TokenNode* getNext(TokenNode *node) {
    return node->next;
}

const char* getTokenType(Tokenizer *tokenizer, TokenNode *node) {
    int tokenType = node->tokenType;

    const char* tokenName = tokenizer->lexemes.at(tokenType).name.c_str();

    return tokenName;
}

int getStart(TokenNode *node) {
    return node->start;
}

int getEnd(TokenNode *node) {
    return node->end;
}

int add(int a, int b) {
    return a + b;
}

const char* testStr(const char* str) {
    return str;
}
}