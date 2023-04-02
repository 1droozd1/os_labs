#include "node_tree.hpp"
#include "net_realization.hpp"
#include <fstream>
#include <signal.h>

int main(int argc, char **argv) {
    if(argc != 3) {
        return -1;
    }
    
    Node task(atoi(argv[1]),atoi(argv[2]));
    std::string prog_path = "./server";
    while(1) {
        std::string message;
        std::string command = " ";
        message = net::reseave_message(&(task.parent));
        std::istringstream  request(message);
        request >> command;

        
        if(command == "create") {
            int id_child, id_parent;
            request >> id_child;
            std::string ans = task.Create(id_child, prog_path);
            net::send_message(&task.parent,ans);
        } else if(command == "pid") {
            std::string ans = task.Pid();
            net::send_message(&task.parent,ans);
        } else if(command == "ping") {
            int id_child;
            request >> id_child;
            std::string ans = task.Ping(id_child);
            net::send_message(&task.parent,ans);
        } else if(command == "send") {
            int id;
            request >> id;
            std::string str;
            getline(request, str);
            str.erase(0,1);
            std::string ans;
            ans = task.Send(str,id);
            net::send_message(&task.parent,ans);
        } else if(command == "exec") {
            int cnt, sum = 0, number;
            request >> cnt;
            for(int i = 0; i < cnt; i++) {
                request >> number;
                sum += number;
            }
            std::string to_send;
            to_send = "Ok: " + std::to_string(task.id) + ": " + std::to_string(sum);
            net::send_message(&task.parent,to_send);
        } else if(command == "remove") {
            std::string ans = task.Remove();
            ans = std::to_string(task.id) + " " + ans;
            net::send_message(&task.parent, ans);
            net::disconnect(&task.parent, task.parent_port);
            task.parent.close();
            break;
        }
    }

    return 0;
}