#include <bits/stdc++.h>
using namespace std;

string time_now();

class TreeNode {
public:
    string name;
    list<string> content;
    char type;
    string cdate;
    string mdate;
    int perm;
    TreeNode* link;
    TreeNode* parent;
    TreeNode* child;

    TreeNode(TreeNode* pwd, string n) {
        name = n;
        cdate = mdate = time_now();
        perm = 6;
        link = NULL;
        child = NULL;
        parent = pwd;
    }

    string get_perm() {
        string res;
        if (perm == 0) res = "---";
        else if (perm == 1) res = "--x";
        else if (perm == 2) res = "-w-";
        else if (perm == 3) res = "-wx";
        else if (perm == 4) res = "r--";
        else if (perm == 5) res = "r-x";
        else if (perm == 6) res = "rw-";
        else if (perm == 7) res = "rwx";

        return res;
    }
};

void linux_tree(TreeNode* root);
void print_help();
void print_tree(TreeNode* root, string prev);
void print_ls(TreeNode* pwd);
void print_stat(TreeNode* root, TreeNode* pwd, string path);
string pwd_str(TreeNode* root, TreeNode* pwd);
list<string> find_names(TreeNode* root, TreeNode* pwd, string name);
TreeNode* find_node(TreeNode* root, TreeNode* pwd, string path);
TreeNode* find_on_pwd(TreeNode* pwd, string name);
list<string> split(string str, char delim);
string join(list<string> str, char delim);
pair<string, string> split_name(const string& str);
TreeNode* cd(TreeNode* root, TreeNode* pwd, string path);
TreeNode* create(TreeNode* root, TreeNode* pwd, string path, char type);
void remove(TreeNode* root, TreeNode* pwd, string path);
void dupl(TreeNode* root, TreeNode* pwd, string src, string dst, int keep);
void edit(TreeNode* root, TreeNode* pwd, string path);
void cat(TreeNode* root, TreeNode* pwd, string path);
void chmod(TreeNode* root, TreeNode* pwd, string path, string new_modes);

int main() {
    TreeNode* root = new TreeNode(NULL, "");
    root->type = 'd';
    TreeNode* pwd = root;
    linux_tree(root);
    print_help();

    string cmd;
    cout << endl << pwd_str(root, pwd) << ">> ";
    while (getline(cin >> ws, cmd)) {
        list<string> args = split(cmd, ' ');
        TreeNode* temp_pwd;
        if (cmd == "help")
            print_help();
        else if (args.front() == "ls") {
            if (args.size() == 1)
                print_ls(pwd->child);
            else while (args.size() != 1) {
                args.pop_front();
                cout << args.front() << ":" << endl;
                temp_pwd = cd(root, pwd, args.front());
                if (temp_pwd != NULL)
                    print_ls(temp_pwd->child);
            }
        } else if (args.front() == "tree") {
            if (args.size() == 1)
                print_tree(pwd->child, "");
            else while (args.size() != 1) {
                args.pop_front();
                cout << args.front() << ":\n.\n";
                temp_pwd = cd(root, pwd, args.front());
                if (temp_pwd != NULL)
                    print_tree(temp_pwd->child, "");
            }

        } else if (cmd == "pwd")
            cout << pwd_str(root, pwd) << endl;
        else if (args.front() == "cd") {
            if (args.size() == 1)
                pwd = root;
            else {
                args.pop_front();
                temp_pwd = cd(root, pwd, args.front());
                if (temp_pwd != NULL)
                    pwd = temp_pwd;
            }

        } else if (args.front() == "find") {
            if (args.size() == 1)
                cout << "find: missing operand" << endl;
            else while(args.size() != 1) {
                args.pop_front();
                list<string> res;
                if (args.front()[0] == '/')
                    res = find_names(root, root, args.front());
                else
                    res = find_names(pwd, pwd, args.front());
                if (res.size() == 0)
                    cout << "find: '" << args.front() << "': no such file or directory" << endl;
                else
                    for (list<string>::iterator it = res.begin(); it != res.end(); it++)
                        cout << *it << endl;
            }
        } else if (args.front() == "stat") {
            if (args.size() == 1)
                cout << "stat: missing operand" << endl;
            else while (args.size() != 1) {
                args.pop_front();
                print_stat(root, pwd, args.front());
            }
        } else if (args.front() == "mkdir") {
            if (args.size() == 1)
                cout << "mkdir: missing operand" << endl;
            else while(args.size() != 1) {
                args.pop_front();
                create(root, pwd, args.front(), 'd');
            }
        } else if (args.front() == "touch") {
            if (args.size() == 1)
                cout << "touch: missing operand" << endl;
            else while(args.size() != 1) {
                args.pop_front();
                create(root, pwd, args.front(), '-');
            }
        } else if (args.front() == "rm" || args.front() == "rmdir") {
            if (args.size() == 1)
                cout << args.front() << ": missing operand" << endl;
            else while(args.size() != 1) {
                args.pop_front();
                remove(root, pwd, args.front());
            }
        } else if (args.front() == "cp" || args.front() == "mv") {
            if (args.size() != 3)
                cout << args.front() << ": missing operand" << endl;
            else {
                int keep;
                if (args.front() == "cp") keep = 1;
                else keep = 0;
                args.pop_front();
                string src = args.front();
                args.pop_front();
                string dst = args.front();
                args.pop_front();
                dupl(root, pwd, src, dst, keep);
            }
        } else if (args.front() == "edit") {
            if (args.size() == 1)
                cout << "edit: missing operand" << endl;
            else {
                args.pop_front();
                edit(root, pwd, args.front());
            }
        } else if (args.front() == "cat") {
            if (args.size() == 1)
                cout << "cat: missing operand" << endl;
            else {
                args.pop_front();
                cat(root, pwd, args.front());
            }
        } else if (args.front() == "chmod") {
            if (args.size() < 3)
                cout << "chmod: missing operand" << endl;
            else {
                args.pop_front();
                string perm = args.front();
                args.pop_front();
                chmod(root, pwd, args.front(), perm);
            }
        } else if (cmd == "exit")
            return 0;
        else
            cout << "Unknown command" << endl;

        cout << endl << pwd_str(root, pwd) << ">> ";
    }

    cout << endl;
    return 0;
}

void print_help() {
    cout << "*** Follows the syntax of linux shell commands ***" << endl << endl;
    cout << "\thelp - print following message" << endl;
    cout << "\tls - list directory" << endl;
    cout << "\tcd - change directory" << endl;
    cout << "\ttree - display the files/directory in tree view" << endl;
    cout << "\tpwd - print working directory" << endl;
    cout << "\tfind - find file/directory path by name" << endl;
    cout << "\tstat - display file/directory information" << endl;
    cout << "\tcp - copy file" << endl;
    cout << "\tmv - move file" << endl;
    cout << "\tmkdir - make directory" << endl;
    cout << "\trmdir - remove directory" << endl;
    cout << "\ttouch - create file" << endl;
    cout << "\trm - remove file" << endl;
    cout << "\tcat - Display file content" << endl;
    cout << "\tedit - edit file" << endl;
    cout << "\tchmod - change file permission" << endl;
    cout << "\texit - exit program" << endl;
}

void print_tree(TreeNode* root, string prev) {
    if (root == NULL) return;

    while (root != NULL) {
        string branch;
        string branch_next;
        
        if (root->link != NULL) {
            branch = "|";
            branch_next = "|";
        } else {
            branch = "`";
            branch_next = " ";
        }

        if (prev == "") {
            cout << branch << "-- " << root->name;
            if (root->type == 'd')
                cout << "/";
            cout << endl;
            prev = branch_next;
        } else {
            cout << prev + "   " + branch << "-- " << root->name;
            if (root->type == 'd')
                cout << "/";
            cout << endl;
            prev += "   " + branch_next;
        }

        print_tree(root->child, prev);

        root = root->link;
        prev = prev.substr(0, prev.size() - 4);
    }
}
void print_ls(TreeNode* root) {
    if (root == NULL) {
        cout << "No such directory" << endl;
        return;
    }

    list<TreeNode*> res;
    TreeNode* temp = root;

    while (temp != NULL) {
        res.push_back(temp);
        temp = temp->link;
    }

    for (auto it = res.begin(); it != res.end(); ++it) {
        cout << (*it)->name;
        cout << " " << (*it)->get_perm() << " " << (*it)->cdate << endl;
    }
}

void print_stat(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* temp = find_node(root, pwd, path);
    if (temp == NULL) {
        cout << "stat: '" << path << "': no such file or directory" << endl;
        return;
    }

    cout << "Name: " << temp->name << endl;
    cout << "Type: " << temp->type << endl;
    cout << "Permissions: " << temp->get_perm() << endl;
    cout << "Creation Date: " << temp->cdate << endl;
    cout << "Modification Date: " << temp->mdate << endl;
}

string pwd_str(TreeNode* root, TreeNode* pwd) {
    string path = "";
    while (pwd != root) {
        path = pwd->name + "/" + path;
        pwd = pwd->parent;
    }
    return "/" + path;
}

list<string> find_names(TreeNode* root, TreeNode* pwd, string name) {
    list<string> res;
    if (root == NULL) return res;

    if (root->name == name)
        res.push_back(pwd_str(root, pwd));

    list<TreeNode*> temp;
    TreeNode* ptr = root;
    while (ptr != NULL) {
        temp.push_back(ptr);
        ptr = ptr->link;
    }

    for (auto it = temp.begin(); it != temp.end(); ++it) {
        res.merge(find_names((*it)->child, (*it), name));
    }

    return res;
}

TreeNode* find_node(TreeNode* root, TreeNode* pwd, string path) {
    list<string> names = split(path, '/');
    if (path[0] == '/')
        root = root;
    else
        root = pwd;

    TreeNode* temp = root;
    string name;
    while (names.size() > 0) {
        name = names.front();
        names.pop_front();
        if (name == "" && names.size() == 0) return temp;

        temp = find_on_pwd(temp, name);
        if (temp == NULL)
            return NULL;
    }

    return temp;
}

TreeNode* find_on_pwd(TreeNode* pwd, string name) {
    TreeNode* temp = pwd->child;
    while (temp != NULL) {
        if (temp->name == name) return temp;
        temp = temp->link;
    }

    return NULL;
}

list<string> split(string str, char delim) {
    list<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

string join(list<string> str, char delim) {
    stringstream ss;
    for (auto it = str.begin(); it != str.end(); ++it) {
        if (it != str.begin()) {
            ss << delim;
        }
        ss << *it;
    }
    return ss.str();
}

pair<string, string> split_name(const string& str) {
    size_t pos = str.find_last_of('/');
    if (pos == string::npos) {
        return make_pair("", str);
    } else {
        return make_pair(str.substr(0, pos), str.substr(pos + 1));
    }
}

TreeNode* cd(TreeNode* root, TreeNode* pwd, string path) {
    if (path == ".") {
        return pwd;
    } else if (path == "..") {
        if (pwd->parent != NULL) {
            return pwd->parent;
        } else {
            cout << "cd: " << path << ": Already at root directory" << endl;
            return pwd;
        }
    } else {
        TreeNode* temp = find_node(root, pwd, path);
        if (temp == NULL) {
            cout << "cd: " << path << ": No such file or directory" << endl;
            return NULL;
        } else if (temp->type != 'd') {
            cout << "cd: " << path << ": Not a directory" << endl;
            return NULL;
        }
        return temp;
    }
}


TreeNode* create(TreeNode* root, TreeNode* pwd, string path, char type) {
    pair<string, string> split_path = split_name(path);
    string parent_path = split_path.first;
    string name = split_path.second;

    TreeNode* new_pwd = cd(root, pwd, parent_path);
    if (new_pwd == NULL) {
        cout << "the directory '" << parent_path << "' does not exist" << endl;
        return NULL;
    }

    TreeNode* temp = find_on_pwd(new_pwd, name);
    string choice = "";
    if (temp != NULL) {
        cout << "the file or directory '" << path << "' already exists" << endl;
        cout << "overwrite? (y/n) ";
        cin >> choice;
        if (choice != "y" && choice != "Y" && choice != "yes" && choice != "Yes")
            return NULL;
        remove(root, new_pwd, name);
    }

    temp = new TreeNode(new_pwd, name);
    temp->type = type;
    temp->parent = new_pwd;

    if (new_pwd->child == NULL) {
        new_pwd->child = temp;
        return temp;
    }
    TreeNode* prev = new_pwd->child;
    while (prev->link != NULL)
        prev = prev->link;
    prev->link = temp;

    return temp;
}

void remove(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* to_remove = find_node(root, pwd, path);
    if (to_remove == NULL) {
        cout << "remove: " << path << ": No such file or directory" << endl;
        return;
    }

    if (to_remove->type == 'd' && to_remove->child != NULL) {
        cout << "remove: " << path << ": Directory not empty" << endl;
        return;
    }

    TreeNode* parent = to_remove->parent;
    TreeNode* temp = parent->child;
    if (temp == to_remove) {
        parent->child = to_remove->link;
    } else {
        while (temp->link != to_remove) {
            temp = temp->link;
        }
        temp->link = to_remove->link;
    }

    delete to_remove;
}

void dupl(TreeNode* root, TreeNode* pwd, string src, string dst, int keep) {
    TreeNode* src_node = find_node(root, pwd, src);
    if (src_node == NULL) {
        cout << "dupl: " << src << ": No such file or directory" << endl;
        return;
    }

    if (find_node(root, pwd, dst) != NULL) {
        cout << "dupl: " << dst << ": File already exists" << endl;
        return;
    }

    TreeNode* dst_node = create(root, pwd, dst, src_node->type);
    if (dst_node == NULL) return;

    if (src_node->type == '-') {
        dst_node->content = src_node->content;
    }

    if (keep == 0)
        remove(root, pwd, src);
}

void edit(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* temp = find_node(root, pwd, path);
    if (temp == NULL) {
        cout << "the file '" << path << "' does not exist" << endl;
        string choice = "";
        cout << "create it? (y/n) ";
        cin >> choice;
        if (choice != "y" && choice != "Y" && choice != "yes" && choice != "Yes")
            return;
        temp = create(root, pwd, path, '-');
    }
    if (temp->type != '-') {
        cout << "'" << path << "' is not a file" << endl;
        return;
    }

    if (!temp->content.empty()) {
        if (temp->perm < 4) { // Read permission (4 -> read)
            cout << "you don't have permission to read '" << path << "'" << endl;
        } else {
            list<string> old_content = temp->content;
            cout << "content: " << endl;
            for (list<string>::iterator it = old_content.begin(); it != old_content.end(); it++)
                cout << *it << endl;
        }

        if (temp->perm < 2 || temp->perm == 4 || temp->perm == 5) { // Write permission (2 -> write)
            cout << "you don't have permission to write '" << path << "'" << endl;
            return;
        }

        string choice = "";
        cout << endl << "overwrite? (y/n) ";
        cin >> choice;
        if (choice != "y" && choice != "Y" && choice != "yes" && choice != "Yes")
            return;
    }

    temp->content.clear();
    cout << endl << temp->name << " : (enter \\n to save)" << endl;
    string line;
    while (1) {
        getline(cin, line);
        if (line == "\\n")
            break;
        temp->content.push_back(line);
    }
    temp->mdate = time_now();
}

void cat(TreeNode* root, TreeNode* pwd, string path) {
    TreeNode* file = find_node(root, pwd, path);
    
    if (file == NULL) {
        cout << "cat: " << path << ": No such file or directory" << endl;
        return;
    }
    
    if (file->type == 'd') {
        cout << "cat: " << path << ": Is a directory" << endl;
        return;
    }
    
    if (file->perm < 4) { // Check if the read permission is present
        cout << "cat: " << path << ": Permission denied" << endl;
        return;
    }
    
    for (const auto& line : file->content) {
        cout << line << endl;
    }
}

void chmod(TreeNode* root, TreeNode* pwd, string path, string new_modes) {
    TreeNode* file = find_node(root, pwd, path);
    if (file == NULL) {
        cout << "chmod: " << path << ": No such file or directory" << endl;
        return;
    }

    int perm = stoi(new_modes);
    file->perm = perm;
}

void linux_tree(TreeNode* root) {
    vector<string> Dirs{"bin", "etc", "etc/systemd", "etc/systemd/system", "etc/systemd/user", 
        "home", "home/user", "home/user/Desktop", "home/user/Documents", "home/user/Downloads", 
        "home/user/Pictures", "tmp"};
    vector<string> Files{"bin/bash", "bin/cd", "bin/gcc", "bin/ls", "bin/python", 
        "etc/systemd/system/display-manager.service", "etc/systemd/system.conf", "etc/systemd/user.conf", 
        "etc/hosts", "etc/passwd", "etc/profile", "home/user/.bashrc"};

    for (string d : Dirs) {
        create(root, root, d, 'd');
    }
    for (string f : Files) {
        create(root, root, f, '-');
    }
}

// Dummy time_now function
string time_now() {
    return "2024-07-27 12:00:00";
}

