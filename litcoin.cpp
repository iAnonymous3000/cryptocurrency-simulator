#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

// Constants
const int MAX_TRANSACTIONS = 15;

// User Class
class User {
public:
    double balance;
    int id;
    string password;

    User() : balance(5.0), id(0), password("") {}
};

unordered_map<int, User> userDatabase;

// Block Structure for Blockchain
struct Block {
    int from;
    int to;
    int amount;
    double hash;
    double hashkey;
    double prev;
    Block* prevPtr;

    Block() : from(0), to(0), amount(0), hash(0), hashkey(0), prev(0), prevPtr(nullptr) {}
};

Block* firstBlock = nullptr;
Block* lastBlock = nullptr;
queue<Block> transactions;

// Hash Function
double hashfunc(int from, int to, int amt) {
    int n = 23;
    return ((from * to) * (n * n * n) + amt);
}

// Get Key Function
double getKey(double hash) {
    if (hash == 0) {
        return 1; // Default key if hash is zero
    }
    for (int i = 1; i < static_cast<int>(hash); i++) {
        int temp = static_cast<int>(hash) / i;
        if (temp % 10 == 0) {
            return i;
        }
    }
    return 1; // Default key if none found
}

// Queue Functions
void enqueue(const Block& b) {
    if (transactions.size() == MAX_TRANSACTIONS) {
        cout << "*** The network has reached its transaction limit ***" << endl;
        return;
    }
    transactions.push(b);
}

Block dequeue() {
    Block temp;
    if (transactions.empty()) {
        cout << "Queue is empty!" << endl;
        return temp;
    }
    temp = transactions.front();
    transactions.pop();
    return temp;
}

// Mining the Blockchain
void mine(int id) {
    if (transactions.empty()) {
        cout << "No pending transactions!" << endl;
        return;
    }
    Block curBlock = dequeue();
    if (userDatabase[curBlock.to].id == 0 || userDatabase[curBlock.from].balance - curBlock.amount < 0 || curBlock.from == curBlock.to) {
        cout << "You have detected an invalid transaction!" << endl;
    } else {
        double curHash = curBlock.hash;
        int k = static_cast<int>(getKey(curHash));
        curBlock.hashkey = k;

        userDatabase[curBlock.from].balance -= curBlock.amount;
        userDatabase[curBlock.to].balance += curBlock.amount;

        Block* newPtr = new Block();
        *newPtr = curBlock;

        if (!firstBlock && !lastBlock) {
            firstBlock = lastBlock = newPtr;
        } else {
            curBlock.prevPtr = lastBlock;
            lastBlock = newPtr;
            curBlock.prev = curBlock.hash;
        }

        cout << "\nYou have authenticated a transaction. You have successfully earned 0.25 LTC" << endl;
        userDatabase[id].balance += 0.25;
    }
}

void sendCoin(int from, int to, int amount) {
    if (userDatabase.find(to) == userDatabase.end()) {
        cout << "User does not exist!" << endl;
        return;
    }
    Block newBlock;
    newBlock.from = from;
    newBlock.to = to;
    newBlock.amount = amount;
    newBlock.hash = hashfunc(from, to, amount);
    enqueue(newBlock);
    cout << "Your transaction has successfully been enqueued!" << endl;
}

int main() {
    int choice, userId;
    string userPass;
    while (true) {
        cout << "\n******* WELCOME TO LitCoin *******" << endl;
        cout << "1. Login\n2. Sign Up\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter your wallet ID: ";
                cin >> userId;
                cout << "Enter your password: ";
                cin >> userPass;

                if (userDatabase[userId].password.compare(userPass) == 0) {
                    int subChoice;
                    while (true) {
                        cout << "\n*******************************" << endl;
                        cout << "What would you like to do next? Choose an option:" << endl;
                        cout << "1. Send LitCoin\n2. Mine a block\n3. Show balance\n4. Log Out" << endl;
                        cin >> subChoice;

                        switch (subChoice) {
                            case 1: {
                                int sendTo, amount;
                                cout << "Enter the wallet ID of the user you want to send it to: ";
                                cin >> sendTo;
                                cout << "Enter the amount of LitCoin you wish to transfer: ";
                                cin >> amount;
                                sendCoin(userId, sendTo, amount);
                                break;
                            }
                            case 2:
                                mine(userId);
                                break;
                            case 3:
                                cout << "Your current unmined balance is: " << userDatabase[userId].balance << " LTC" << endl;
                                break;
                            case 4:
                                goto END_SESSION;
                        }
                    }
                } else {
                    cout << "Wrong username or Password!" << endl;
                }
                break;
            case 2: {
                string newPassword;
                User newUser;
                newUser.id = userDatabase.size() + 1;
                cout << "Hello, thank you for signing up! Your Wallet ID is " << newUser.id << endl;
                cout << "Enter a desired password: ";
                cin >> newPassword;
                newUser.password = newPassword;
                userDatabase[newUser.id] = newUser;
                cout << "You have successfully registered for LitCoin." << endl;
                cout << "Your current balance is " << newUser.balance << " LTC" << endl;
                break;
            }
            default:
                cout << "Please enter a valid option!" << endl;
                break;
        }
    }
END_SESSION:
    return 0;
}
