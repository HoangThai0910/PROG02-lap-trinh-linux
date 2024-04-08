#include <bits/stdc++.h>

using namespace std;

struct UserInfo {
    string username;
    string uid;
    string gid;
    string home;
};

vector<UserInfo> readpasswdfile(string filepath) {
    vector<UserInfo> users;
    ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Failed to open /etc/passwd file." << endl;
        return users;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        vector<string> tmp;
        while (getline(iss, token, ':')) {
            tmp.push_back(token);
        }

        if (tmp.size() >= 6) {
            UserInfo user;
            user.username = tmp[0];
            user.uid = tmp[2];
            user.gid = tmp[3];
            user.home = tmp[5];
            users.push_back(user);
        }
    }

    file.close();
    return users;
}

vector<string> readgroupfile(string filepath, string name){
    vector<string> grouplist;
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open /etc/group file." << endl;
        return grouplist;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        vector<string> tmp;
        while (getline(iss, token, ':')) {
            tmp.push_back(token);
        }
        if (tmp[tmp.size()-1].find(name) < tmp.size())
        {
            grouplist.push_back(tmp[0]);
        }
        
    }
    file.close();
    return grouplist;
}

int main() {
    vector<UserInfo> users = readpasswdfile("/etc/passwd");
    string name;
    cin>>name;
    vector<string> grouplist = readgroupfile("/etc/group",name);
    for(auto it:users){
        if(name==it.username){
            cout<<"Username: "<<it.username<<endl;
            cout<<"User ID: "<<it.uid<<endl;
            cout<<"Group ID: "<<it.gid<<endl;
            cout<<"Thu muc home: "<<it.home<<endl;
            cout<<"Danh sach nhom cua user:"<<endl;
            for(string x:grouplist){
                cout<<x<<", ";
            }
	    cout<<endl;
        }
    }
    return 0;
}
