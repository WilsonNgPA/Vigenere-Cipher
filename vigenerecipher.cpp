//g++ -pedantic -Wall -Wextra -std=c++14 vigenerecipher.cpp -o cipher
//This program will perform the encryption step of Vigenere Cipher.

#include <iostream>
#include <cctype>
#include <fstream>

//inputs: str - a string to be capitalized 
//no outputs as this method modifies the original string itself
//This method converts every alphabet in the string to uppercase
void capitalize(std::string &str) {
    for(char &c : str) {
        c = std::toupper(c);
    }
}

//inputs: key - the cipher key entered by the user
//output: true if the key entered by user only contains alphabets and false if it contains any alphabets
//This method checks every character in a string for any non alphabets
bool hasAlphabetsOnly(std::string key) {
    for(unsigned i = 0; i < key.length(); i++) {
        if(key[i] < 'A' || key[i] > 'Z') {
            return false;
        }
    }
    return true;
}

//inputs: key - an empty string variable in the main function where the key is to be stored
//no outputs as this method modifies the original string itself
//This method prompts the user to enter a key that only contains alphabets and stores the key in the main function
void getKey(std::string &key) { 
    std::cout << "Enter your encryption key: " << std::endl;
    std::cin >> key;
    capitalize(key);
    while(key.empty() || !hasAlphabetsOnly(key)) {
        std::cout << "Enter your encryption key that contains only alphabets: " << std::endl;
        std::cin >> key;
        capitalize(key);
    }
}

//inputs: filename - a string entered by the user containing the file name
//outputs: a string containing the text in the file specified by the user
/*
This method looks for the file specified by the user, opens it and read 
in the message line by line until end of file. If file does not exist, 
the program will exit
*/ 
std::string getMsg(const std::string &filename) {
    std::ifstream in;
    std::string temp;
    std::string message;
    in.open(filename);
    if(in.is_open()) {
        while(!in.eof()) {
            getline(in, temp);
            message += temp + " ";
        }
        in.close();
    }
    else {
        std::cout << "The file specified does not exists" << std::endl;
        exit(0);
    }
    return message;
}

//inputs: msg - a string containing the message in the file specified by user
//outputs: a string containing the message but without any non-alphanumeric characters
//This methods copies a string leaving out any non-alphanumeric character and returns it
std::string removeNonAlphaNumeric(std::string msg) {
    std::string str;
    capitalize(msg);
    for(unsigned i = 0; i < msg.length(); i++) {
        if((msg[i] >= 'A' && msg[i] <= 'Z') || (msg[i] >= '0' && msg[i] <= '9')) {
            str += msg[i];
        }
    }
    return str;
}


/*
inputs: msg - a string containing the message to be encrypted. 
key - a string containing the key entered by the user
*/
//outputs: a string containing a new key with the same length as the message
/*
This method takes in a msg and a key, checks the size of both msg and key.
If the msg is shorter than the key, exit program
If the msg is same size as key, return key
Else, this method will cycle the key until it reaches the same length as message, 
skipping any numbers in the message
*/
std::string cycleKey(std::string msg, std::string key) {
    std::string newKey = "";
    if(msg.size() == key.size()) {
        newKey = key;
    }
    else if(msg.size() == 0) {
        std::cout << "Message contains no alphanumeric characters, program will now exit" << std::endl;
        exit(0);
    }
    else {
        for(unsigned i = 0, j = 0; i < msg.size(); i++, j++) {
            if(j == key.size()) {
                j = 0;
            }
            if(msg[i] >= 'A' && msg[i] <= 'Z') {
                newKey += key[j];
            }
            //If msg[i] is a number, add a ' ' instead and decrement j to maintain key position
            else {
                newKey += ' ';
                j -= 1;
            }
        }
    }
    return newKey;
}

/*
inputs: og - the char to be shifted. 
shift - a char that determines how much og is shifted by
*/
//outputs: a char that is og shifted by shift
//This method shifts og by shift and returns it.
char shiftChar(char og, char shift) {
    char newChar;
    if(og >= '0' && og <= '9') {
        newChar = og;
    }
    else {
        int c = (og + shift) % 26;
        newChar = c + 'A';
    }
    return newChar;
}

/*
inputs: msg - a string that contains the message without alphanumeric characters, capitalized.
key - a string that contains the new key generated to match the length of msg
*/
//outputs: a string containing the encrypted message
/*
This methods encrypts the message in the file specified by the user by using a for loop to go 
through each character in the msg and using the shiftChar() method to shift the character.
The shifted characters are added to a new string which is then returned by this method.
*/
std::string encrypt(std::string msg, std::string key) {
    std::string encryptedStr = "";
    for(unsigned i = 0; i < msg.length(); i++) {
        //If key[i] is a ' ', that means that msg[i] is a number, so just add msg[i] to the string instead
        if(key[i] == ' ') {
            encryptedStr += msg[i];
        }
        else {
            encryptedStr += shiftChar(msg[i],key[i]);
        }
    }
    return encryptedStr;
}


int main(){
    //Creating a variable to store the key and calling the getKey() method to get the key from user
    std::string key;
    getKey(key);

    //Creating a variable to store the filename and prompting the user for the filename
    std::string filename; 
    std::cout << "What file do you wish to Encrypt?" << std::endl;
    std::cin >> filename;

    //Creating a variable to store the message in the file without non-alphanumeric characters
    std::string formattedMessage = removeNonAlphaNumeric(getMsg(filename));

    //Creating a variable to store the new key generated to match the length of the formatted message
    std::string newKey = cycleKey(formattedMessage, key);

    //Outputting the results of the encryption to the user
    std::cout << encrypt(formattedMessage, newKey);
}
