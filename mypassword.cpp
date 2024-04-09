#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <pwd.h>
#include <shadow.h>
#include <sstream>

using namespace std;

//Kiem tra mat khau cu
bool authenticateUser(const string& username, const string& oldPassword) {
    ifstream passwdFile("/etc/passwd");
    if (!passwdFile.is_open()) {
        cerr << "Khong the mo file open /etc/passwd." << endl;
        return false;
    }
    string line;
    while (getline(passwdFile, line)) {
        stringstream ss(line);
        string usernameInFile;
        getline(ss, usernameInFile, ':');
        if (usernameInFile == username) {
            string password;
            getline(ss, password, ':');
            //Neu mat khau duoc luu trong file /etc/shadow
            if (password=="x"){
                struct spwd *sp;
    		    sp = getspnam(username.c_str());
    		    if (sp == NULL) {
        	        cerr << "Khong tim thay nguoi dung" << endl;
         	        return false;
    		    }
	    	    char *hashedPassword = sp->sp_pwdp;
	    	    if (strcmp(crypt(oldPassword.c_str(), hashedPassword), hashedPassword) == 0) {
		            return true;
	    	    } else return false;
            } else{
                //Neu mat khau duoc luu trong file /etc/passwd
            	char *encryptedPassword = crypt(oldPassword.c_str(), "$6$saltstring$");
            	if (password==encryptedPassword){
            	    return true;
            	} else return false;
            }
        }
    }
    return false;
}

//Doi mat khau moi
bool changePassword(const string& username, const string& newPassword) {
    char *encryptedPassword = crypt(newPassword.c_str(), "$6$saltstring$");
    // Mo file /etc/passwd
    ifstream passwdFile("/etc/passwd");
    if (!passwdFile.is_open()) {
        cerr << "Khong the mo file /etc/passwd." << endl;
        return false;
    }
    // Tao file /etc/passwd tam thoi
    ofstream tempFile("/etc/passwd.temp");
    if (!tempFile.is_open()) {
        cerr << "Khon the tao file." << endl;
        passwdFile.close();
        return false;
    }
    string line;
    while (getline(passwdFile, line)) {
        stringstream ss(line);
        string usernameInFile;
        getline(ss, usernameInFile, ':');
        if (usernameInFile == username) {
            //Thay doi mat khau cua nguoi dung trong file /etc/passwd
            string oldpassword, uid, gid, gecos, home, shell;
            getline(ss, oldpassword, ':');
            getline(ss, uid, ':');
            getline(ss, gid, ':');
            getline(ss, gecos, ':');
            getline(ss, home, ':');
            getline(ss, shell, ':');
            tempFile << username << ":" << encryptedPassword << ":" << uid << ":" << gid << ":" << gecos << ":" << home << ":" << shell << endl;
        } else {
            // Neu la nguoi dung khac giu nguyen thong tin
            tempFile << line << endl;
        }
    }
    passwdFile.close();
    tempFile.close();

    // Thay the file /etc/passwd voi file tam thoi vua tao
    if (rename("/etc/passwd.temp", "/etc/passwd") != 0) {
        cerr << "Khong the cap nhat file /etc/passwd" << endl;
        return false;
    }
    
    return true;
}

int main() {
    string username, oldPassword, newPassword;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your old password: ";
    cin >> oldPassword;

    if (!authenticateUser(username, oldPassword)) {
        cerr << "Authentication failed. Incorrect username or password." << endl;
        return 1;
    }
    
    cout << "Enter your new password: ";
    cin >> newPassword;

    if (changePassword(username, newPassword)) {
        cout << "Password changed successfully." << endl;
    } else {
        cerr << "Failed to change password." << endl;
        return 1;
    }
    
    return 0;
}
