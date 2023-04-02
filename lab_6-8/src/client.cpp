#include "node_tree.hpp"
#include "net_realization.hpp"
#include "set"
#include <signal.h>

int main() {
    std::set<int> all_nodes;
    std::string prog_path = "./server";
    Node task(-1);
    all_nodes.insert(-1);
    std::string command;
    while(std::cin >> command) {
        if(command == "create") {
            int id_child, id_parent;
            std::cin >> id_child >> id_parent;
            if(all_nodes.find(id_child) != all_nodes.end()) {
                std::cout << "Error: Already exists" << std::endl;
            } else if(all_nodes.find(id_parent) == all_nodes.end()) {
                std::cout << "Error: Parent not found" << std::endl;
            }else if(id_parent == task.id) {
                std::string ans = task.Create(id_child, prog_path);
                std::cout << ans << std::endl;
                all_nodes.insert(id_child);
            } else{
                std::string str = "create " + std::to_string(id_child);
                std::string ans = task.Send(str, id_parent);
                std::cout << ans << std::endl;
                all_nodes.insert(id_child);
            }   
        } else if(command == "ping") {
            int id_child;
            std::cin >> id_child;
            if(all_nodes.find(id_child) == all_nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            }else if(task.children.find(id_child) != task.children.end()) {
                std::string ans = task.Ping(id_child);
                std::cout << ans << std::endl;
            }else{
                std::string str = "ping " + std::to_string(id_child);
                std::string ans = task.Send(str, id_child);
                if(ans == "Error: not find") {
                    ans = "Ok: 0";
                }
                std::cout << ans << std::endl;
            }
        }else if(command == "exec") {
            int id, number, cnt;
            std::cin >> id >> cnt;
            std::string msg = "exec " + std::to_string(cnt); 
            for(int i = 0; i < cnt; i++) {
                std::cin >> number;
                msg += " " + std::to_string(number);
            }
            if(all_nodes.find(id) == all_nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            }else{
                std::string ans = task.Send(msg,id);
                std::cout << ans << std::endl;
            }
        }else if(command == "remove") {
            int id;
            std::cin >> id;
            std::string msg = "remove";
            if(all_nodes.find(id) == all_nodes.end()) {
                std::cout << "Error: Not found" << std::endl;
            }else{
                std::string ans = task.Send(msg,id);
                if(ans != "Error: not find") {
                    std::istringstream ids(ans);
                    int tmp;
                    while(ids >> tmp) {
                        all_nodes.erase(tmp);
                    }
                    ans = "Ok";
                    if(task.children.find(id) != task.children.end()) {
                        net::unbind(task.children[id],task.children_port[id]);
                        task.children[id]->close();
                        task.children.erase(id);
                        task.children_port.erase(id);
                    }
                }
                std::cout << ans << std::endl;
            }
        } else if(command == "exit") {
            task.Remove();
            return 0;
        }
    }
}