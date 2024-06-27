#include <iostream>
#include <string>
using namespace std;

// Node for the linked list of grades
struct GradeNode {
    double grade;
    GradeNode* next;

    GradeNode(double g) : grade(g), next(nullptr) {}
};

// Node for the Binary Search Tree
struct BSTNode {
    string info;
    BSTNode* left;
    BSTNode* right;
    GradeNode* grades;

    BSTNode(string subject) : info(subject), left(nullptr), right(nullptr), grades(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    // Function to insert a grade into the linked list
    void insertGrade(GradeNode*& node, double grade) {
        if (grade < 0 || grade > 100) {
            cout << "Invalid grade! Grades must be between 0 and 100.\n";
            return;
        }
        GradeNode* newGrade = new GradeNode(grade);
    
        if (node == nullptr || grade < node->grade) {
            newGrade->next = node;
            node = newGrade;
            return;
        }
    
        GradeNode* current = node;
        GradeNode* prev = nullptr;
    
        while (current != nullptr && grade >= current->grade) {
            prev = current;
            current = current->next;
        }
    
        newGrade->next = current;
        
        if (prev != nullptr) {
            prev->next = newGrade;
        } else {
            node = newGrade;
        }
    }

    // Function to insert a subject into the BST
    void insertSubject(BSTNode*& node, string subject) {
        if (node == nullptr) {
            node = new BSTNode(subject);
            return;
        }
        
        if (subject < node->info) {
            if (node->left == nullptr) {
                node->left = new BSTNode(subject);
            } else {
                insertSubject(node->left, subject);
            }
        } else if (subject > node->info) {
            if (node->right == nullptr) {
                node->right = new BSTNode(subject);
            } else {
                insertSubject(node->right, subject);
            }
        } else {
            cout << "Subject '" << subject << "' already exists in the BST.\n";
        }
    }

    // Function to print subjects and their grades
    void printSubjects(BSTNode* node) {
        if (node != nullptr) {
            printSubjects(node->left);
            cout << node->info << ": ";
            GradeNode* currentGrade = node->grades;
            double totalGrade = 0.0;
            int count = 0;
            while (currentGrade != nullptr) {
                cout << currentGrade->grade << " ";
                totalGrade += currentGrade->grade;
                count++;
                currentGrade = currentGrade->next;
            }
            double average = count > 0 ? totalGrade / count : 0.0;
            cout << "(Avg: " << average << ")\n";
            printSubjects(node->right);
        }
    }

    // Function to find a subject in the BST
    bool findSubject(BSTNode* node, string subject) {
        if (node == nullptr) {
            return false;
        }
        if (subject < node->info) {
            return findSubject(node->left, subject);
        } else if (subject > node->info) {
            return findSubject(node->right, subject);
        }
        return true;
    }

    // Function to remove a subject from the BST
    void removeSubject(BSTNode*& node, string subject) {
        if (node == nullptr) {
            cout << "Not found\n";
            return;
        }
        if (subject < node->info) {
            removeSubject(node->left, subject);
        } else if (subject > node->info) {
            removeSubject(node->right, subject);
        } else {
            deleteNode(node);
            cout << "Subject '" << subject << "' has been removed.\n";
        }
    }

    // Function to delete a node from the BST
    void deleteNode(BSTNode*& node) {
        if (node == nullptr) {
            return;
        }
        
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            deleteGrades(node->grades);
            delete node;
            node = temp;
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            deleteGrades(node->grades);
            delete node;
            node = temp;
        } else {
            BSTNode* successorParent = node;
            BSTNode* successor = node->right;
    
            while (successor->left != nullptr) {
                successorParent = successor;
                successor = successor->left;
            }
    
            node->info = successor->info;
            deleteGrades(successor->grades);
    
            if (successorParent == node) {
                successorParent->right = successor->right;
            } else {
                successorParent->left = successor->right;
            }
            delete successor;
        }
    }

    // Function to find the minimum node in the subtree
    BSTNode* findMin(BSTNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Function to remove a grade from a subject
    void removeGrade(BSTNode* node, double grade, string subject) {
        while (node != nullptr) {
            if (subject < node->info) {
                node = node->left;
            } else if (subject > node->info) {
                node = node->right;
            } else {
                GradeNode* current = node->grades;
                GradeNode* prev = nullptr;
                while (current != nullptr && current->grade != grade) {
                    prev = current;
                    current = current->next;
                }
                if (current != nullptr) {
                    if (prev == nullptr) {
                        node->grades = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    delete current;
                    cout << "Grade " << grade << " has been removed from subject '" << subject << "'.\n";
                    return;
                } else {
                    cout << "Grade not found for the subject.\n";
                    return;
                }
            }
        }
        cout << "Subject not found.\n";
    }

    // Function to delete all grades in a linked list
    void deleteGrades(GradeNode* node) {
        while (node != nullptr) {
            GradeNode* temp = node;
            node = node->next;
            delete temp;
        }
    }

    // Recursive helper function to find maximum grade in all subjects
    void maxGradeRecursive(BSTNode* node, double& maxGradeSoFar, string& subjectWithMaxGrade) {
        if (node == nullptr) {
            return;
        }

        GradeNode* currentGrade = node->grades;
        while (currentGrade != nullptr) {
            if (currentGrade->grade > maxGradeSoFar) {
                maxGradeSoFar = currentGrade->grade;
                subjectWithMaxGrade = node->info;
            }
            currentGrade = currentGrade->next;
        }

        maxGradeRecursive(node->left, maxGradeSoFar, subjectWithMaxGrade);
        maxGradeRecursive(node->right, maxGradeSoFar, subjectWithMaxGrade);
    }

public:
    BST() : root(nullptr) {}

    // Function to get the root of the BST
    BSTNode* getRoot() {
        return root;
    }

    // Function to insert a subject into the BST
    void insertSubject(string subject) {
        if (root == nullptr) {
            root = new BSTNode(subject);
        } else {
            insertSubject(root, subject);
        }
    }

    // Function to insert a grade into a subject
    void insertGrade(double grade, string subject) {
        BSTNode* node = root;
        bool found = false;
    
        while (node != nullptr) {
            if (subject < node->info) {
                node = node->left;
            } else if (subject > node->info) {
                node = node->right;
            } else {
                found = true;
                insertGrade(node->grades, grade);
                break;
            }
        }
    
        if (!found) {
            cout << "Subject '" << subject << "' does not exist in the BST. Please add the subject first.\n";
        }
    }

    // Function to print all subjects with their grades
    void printSubjects() {
        printSubjects(root);
    }

    // Function to find a subject in the BST
    bool findSubject(string subject) {
        return findSubject(root, subject);
    }

    // Function to remove a subject from the BST
    void removeSubject(string subject) {
        removeSubject(root, subject);
    }

    // Function to remove a grade from a subject
    void removeGrade(double grade, string subject) {
        removeGrade(root, grade, subject);
    }

    // Function to print the BST in post-order traversal
    void printBSTPostOrder(BSTNode* node) {
        postOrderTraversal(node);
        cout << "\n";
    }

    // Helper function for post-order traversal
    void postOrderTraversal(BSTNode* node) {
        if (node == nullptr) {
            return;
        }
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        cout << node->info << " ";
    }

    // Function to find the maximum grade in all subjects
    void maxGradeInAll() {
        double maxGradeSoFar = -1;
        string subjectWithMaxGrade = "";
        maxGradeRecursive(root, maxGradeSoFar, subjectWithMaxGrade);

        // Print the maximum grade and corresponding subject
        cout << "Maximum grade in all subjects: ";
        if (maxGradeSoFar != -1) {
            cout << maxGradeSoFar << " (Subject: " << subjectWithMaxGrade << ")\n";
        } else {
            cout << "No grades found.\n";
        }
    }
};

// Main function
int main() {
    BST bst;

    int choice;
    string subject;
    double grade;
    bool validChoice = false;

    // Main menu loop
    do {
        cout << "Enter 1 to add a new subject\n";
        cout << "Enter 2 to add a new grade in a subject\n";
        cout << "Enter 3 to print all subjects with grades in each subject\n";
        cout << "Enter 4 to find a subject\n";
        cout << "Enter 5 to remove a subject\n";
        cout << "Enter 6 to remove a grade from a subject\n";
        cout << "Enter 7 to find the maximum grade in all subjects\n";
        cout << "Enter 8 to display the Binary Search Tree in Post-Order\n";
        cout << "Enter 9 to exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number between 1 and 9.\n";
            continue;
        }

        if (choice < 1 || choice > 9) {
            cout << "Invalid choice. Please enter a number between 1 and 9.\n";
            continue;
        }

        validChoice = true;

        // Switch case to perform selected operation
        switch (choice) {
            case 1:
                cout << "Enter the subject name: ";
                cin.ignore();
                getline(cin, subject);
                bst.insertSubject(subject);
                break;
            case 2:
                cout << "Enter the subject name: ";
                cin.ignore();
                getline(cin, subject);
                cout << "Enter the grade: ";
                cin >> grade;
                bst.insertGrade(grade, subject);
                break;
            case 3:
                cout << "Subjects and their grades:\n";
                bst.printSubjects();
                break;
            case 4:
                cout << "Enter the subject to find: ";
                cin.ignore();
                getline(cin, subject);
                if (bst.findSubject(subject)) {
                    cout << "Subject found in the BST.\n";
                } else {
                    cout << "Subject not found in the BST.\n";
                }
                break;
            case 5:
                cout << "Enter the subject to remove: ";
                cin.ignore();
                getline(cin, subject);
                bst.removeSubject(subject);
                break;
            case 6:
                cout << "Enter the subject name: ";
                cin.ignore();
                getline(cin, subject);
                cout << "Enter the grade to remove: ";
                cin >> grade;
                bst.removeGrade(grade, subject);
                break;
            case 7:
                bst.maxGradeInAll();
                break;
            case 8:
                cout << "Binary Search Tree:\n";
                bst.printBSTPostOrder(bst.getRoot());
                break;
            case 9:
                char confirm;
                cout << "Are you sure you want to exit? (Y/N): ";
                cin >> confirm;
                if (confirm == 'Y' || confirm == 'y') {
                    cout << "Exiting the program.\n";
                    return 0; // Exiting the program
                } else {
                    validChoice = false; // Continue the loop if user chooses not to exit
                }
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }

    } while (choice != 9 || !validChoice);

    return 0;
}