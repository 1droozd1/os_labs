#include <iostream>
#include "net_realization.hpp"
#include <sstream>
#include <unordered_map>
#include "unistd.h"

class Node{
private:
    zmq::context_t context;
public:
    std::unordered_map<int,zmq::socket_t*> children;
    std::unordered_map<int,int> children_port;
    zmq::socket_t parent;
    int parent_port;

    int id;
    Node(int _id , int _parent_port = -1):id(_id),parent(context,ZMQ_REP),parent_port(_parent_port) {
        if(_id != -1) {
            net::connect(&parent,_parent_port);
        }
    }

    std::string Ping(int _id) {
        std::string ans = "Ok: 0";
        if(_id == id) {
            ans = "Ok: 1";
            return ans;
        } else if(children.find(_id) != children.end()) {
            std::string msg = "ping " + std::to_string(_id);
            net::send_message(children[_id],msg);
            try{
                msg  = net::reseave_message(children[_id]);
                if(msg == "Ok: 1")
                    ans = msg;
            } catch(int) {}
            return ans;
        }else{
            return ans;
        }
    } 

    std::string Create(int child_id,std::string program_path) {
        std::string program_name = program_path.substr(program_path.find_last_of("/") + 1);
        children[child_id] = new zmq::socket_t(context,ZMQ_REQ);
        
        int new_port = net::bind(children[child_id],child_id);
        children_port[child_id] = new_port;
        int pid = fork();
        
        if(pid == 0) {
            execl(program_path.c_str(), program_name.c_str(), std::to_string(child_id).c_str() , std::to_string(new_port).c_str() ,(char*)NULL);
        }else{
            std::string child_pid;
            try{
                children[child_id]->setsockopt(ZMQ_SNDTIMEO,3000);
                net::send_message(children[child_id],"pid");
                child_pid = net::reseave_message(children[child_id]);
            } catch(int) {
                child_pid = "Error: can't connect to child";
            }
            return "Ok: " + child_pid;
        }
    }

    std::string Pid() {
        return std::to_string(getpid());
    }

    std::string Send(std::string str,int _id) {
        if(children.size() == 0) {
            return "Error: not find";
        }else if(children.find(_id) != children.end()) {
            if(Ping(_id) == "Ok: 1") {
                net::send_message(children[_id],str);
                std::string ans;
                try{
                    ans = net::reseave_message(children[_id]);
                } catch(int) {
                    ans = "Error: not find";
                }
                return ans;
            }
        }else{
            std::string ans = "Error: not find";
            for(auto& child: children ) {
                if(Ping(child.first) == "Ok: 1") {
                    std::string msg = "send " + std::to_string(_id) + " " + str;
                    net::send_message(children[child.first],msg);
                    try{
                        msg = net::reseave_message(children[child.first]);
                    } catch(int) {
                        msg = "Error: not find";
                    }
                    if(msg != "Error: not find") {
                        ans = msg;
                    }
                }
            }
            return ans;
        }
    }

    std::string Remove() {
        std::string ans;
        if(children.size() > 0) {
            for(auto& child: children ) {
                if(Ping(child.first) == "Ok: 1") {
                    std::string msg = "remove";
                    net::send_message(children[child.first],msg);
                    try{
                        msg = net::reseave_message(children[child.first]);
                        if(ans.size() > 0)
                            ans = ans + " " + msg;
                        else
                            ans =  msg;
                    } catch(int) {}
                }
                net::unbind(children[child.first], children_port[child.first]);
                children[child.first]->close();
            }
            children.clear();
            children_port.clear();
        }
        return ans;
    }
};
