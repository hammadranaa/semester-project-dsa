#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

struct FamilyMember {
    string name;
    FamilyMember* firstChild;
    FamilyMember* nextSibling;
    FamilyMember(string memberName) :
        name(memberName),
        firstChild(nullptr),
        nextSibling(nullptr) {}
};

class FamilyTree {
private:
    unordered_map<string, FamilyMember*> members;
    FamilyMember* root;

    void deleteTree(FamilyMember* member) {
        if (!member) return;
        deleteTree(member->nextSibling);
        deleteTree(member->firstChild);
        delete member;
    }

    void displayTree(FamilyMember* member, int level = 0) const {
        if (!member) return;
        // Print current member with proper indentation
        for (int i = 0; i < level; ++i) {
            cout << "    ";
        }
        cout << "--- " << member->name << endl;
        // Print all children
        displayTree(member->firstChild, level + 1);
        // Print all siblings
        displayTree(member->nextSibling, level);
    }

    void writeTreeToFile(FamilyMember* member, ofstream& file, int level = 0) const {
        if (!member) return;
        // Write current member with proper indentation
        for (int i = 0; i < level; ++i) {
            file << "    ";
        }
        file << "--- " << member->name << endl;
        // Write all children
        writeTreeToFile(member->firstChild, file, level + 1);
        // Write all siblings
        writeTreeToFile(member->nextSibling, file, level);
    }

public:
    FamilyTree(string rootName) {
        root = new FamilyMember(rootName);
        members[rootName] = root;
    }

    void addChild(string parentName, string childName) {
        if (members.find(parentName) == members.end()) {
            cout << "Parent " << parentName << " not found!\n";
            return;
        }
        FamilyMember* parent = members[parentName];
        FamilyMember* child = new FamilyMember(childName);
        members[childName] = child;
        if (!parent->firstChild) {
            parent->firstChild = child;
        }
        else {
            FamilyMember* current = parent->firstChild;
            while (current->nextSibling) {
                current = current->nextSibling;
            }
            current->nextSibling = child;
        }
    }

    void displayTree() const {
        cout << "\nFamily Tree Structure:\n";
        cout << root->name << " (Root)" << endl;
        displayTree(root->firstChild, 1);
    }

    void saveTreeToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to open file " << filename << " for writing.\n";
            return;
        }

        file << "Family Tree Structure:\n";
        file << root->name << " (Root)" << endl;
        writeTreeToFile(root->firstChild, file, 1);

        file.close();
        cout << "\nFamily tree has been saved to " << filename << endl;
    }

    ~FamilyTree() {
        deleteTree(root);
    }

    unordered_map<string, FamilyMember*>& getMembers() {
        return members;
    }
};

int main() {
    string rootName;
    cout << "Enter the name of the root family member (Generation 0): ";
    getline(cin, rootName);
    FamilyTree family(rootName);

    for (int gen = 1; gen <= 3; ++gen) {
        cout << "\nGeneration " << gen << ":\n";
        cout << "How many family members do you want to add to this generation? ";
        int numMembers;
        cin >> numMembers;
        cin.ignore();

        for (int i = 0; i < numMembers; ++i) {
            string parentName;
            cout << "\nFor member " << (i + 1) << " of generation " << gen << ":\n";
            cout << "Enter their parent's name: ";
            getline(cin, parentName);

            if (family.getMembers().find(parentName) == family.getMembers().end()) {
                cout << "Parent not found. Please make sure to enter a valid parent's name.\n";
                i--; // Retry this iteration
                continue;
            }

            string childName;
            cout << "Enter their name: ";
            getline(cin, childName);
            family.addChild(parentName, childName);
        }
    }

    // Display the complete family tree on console
    family.displayTree();

    // Save the family tree to a file
    family.saveTreeToFile("family_tree.txt");

    return 0;
}