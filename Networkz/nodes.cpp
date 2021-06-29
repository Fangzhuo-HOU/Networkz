#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Node {
public:
    int nId;
    map<string, string> nAttribute;
    map<int, string> nAdjacent;

    string getAttribute(string key) {
        return nAttribute[key];
    }

    vector<string> getAdjacentAttr(int nId) {
        string r = nAdjacent[nId];
        int starIndex = r.find('*');
        string weight = r.substr(0, starIndex);
        string type = r.substr(starIndex+1, r.size());
        vector<string> s = {weight, type};
        return s;
    }

    int showAllAttributes() {
        cout << "Attributes: || ";
        for (auto kv : nAttribute) {
            cout << kv.first << ":" << kv.second << " | ";
        }
        cout << " " << endl;
        return 0;
    }

    int showAllAdjacent() {
        for (auto kv : nAdjacent) {
            cout << "adjacent Node: " << kv.first << "   weight-type: " << kv.second << endl;

        }
        return 0;
    }
};

class linkage {
public:
    string node1;
    string node2;
    string type;
    int weight;

    int printLinkage() {
        cout << node1 + "——" + node2 + " | " + type + " | " + to_string(weight) << endl;
        return 0;
    }
};

class Utils {
public:
    vector<string> findNodeAttr(string stkcd, vector<vector<string>> attributes) {
        vector<string> k;
        for (int i = 0; i <= attributes.size(); i++) {
            if (attributes[i][0] == stkcd) {
                return attributes[i];
            }
        }
        return k;
    }

    map<int, string> findAdjNode(string stkcd, vector<linkage> linkages) {
        map<int, string> nAdjNode;

        for (auto l : linkages) {

            if (l.node1 == stkcd) {
                nAdjNode.insert(pair<int, string>(atoi(l.node2.c_str()), to_string(l.weight) + "*" + l.type));
                continue;
            }
            if (l.node2 == stkcd) {
                nAdjNode.insert(pair<int, string>(atoi(l.node1.c_str()), to_string(l.weight) + "*" + l.type));
                continue;
            }


        }

        return nAdjNode;
    }

public:
    vector<linkage> getLinkage(string filePath) {

//        system("chcp 65001");
        Utils u;
        vector<vector<string>> pairArray = u.getRawData(filePath);
        vector<vector<string>> nodeAttrs;

        vector<linkage> pair;

        for (int i = 0; i < pairArray.size(); i++) {
            int key[3];
            for (int j = 0; j < pairArray[i].size(); j++) {
                key[j] = atoi(pairArray[i][j].c_str());
            }
            if (key[1] < key[0]) {
                int temp = key[1];
                key[1] = key[0];
                key[0] = temp;
            }
            linkage l;
            l.node1 = to_string(key[0]);
            l.node2 = to_string(key[1]);
            l.type = to_string(key[2]);
            l.weight = 1;

            int flag = 0;
            for (int i = 0; i < pair.size(); i++) {
                if (pair[i].node1 == l.node1 && pair[i].node2 == l.node2 && pair[i].type == l.type) {
                    pair[i].weight = pair[i].weight + 1;
                    flag = flag + 1;
                    break;
                }
            }

            if (flag == 0) {
                pair.push_back(l);
            }
        }

        return pair;
    }

public:
    vector<vector<string>> getRawData(string filePath) {
        ifstream inFile(filePath, ios::in);
        string lineStr;
        vector<vector<string>> strArray;
        while (getline(inFile, lineStr)) {
            stringstream ss(lineStr);
            string str;
            vector<string> lineArray;
            while (getline(ss, str, ','))
                lineArray.push_back(str);
            strArray.push_back(lineArray);
        }

        return strArray;
    }
};

class Network {
public:
    vector<Node> NODES;
    int nodeNum;

    int initializeNetwork(string linkageFilePath, string attributesFilePath) {
        Utils u;
        vector<string> attrName = {"stkcd", "color", "status", "price"};
        vector<Node> nodes;
        vector<string> ns;
        int id = 0;


        vector<linkage> linkages = u.getLinkage(linkageFilePath);


        vector<vector<string>> attributes = u.getRawData(attributesFilePath);


        for (auto item : linkages) {
            ns.push_back(item.node1);
            ns.push_back(item.node2);
        }
        sort(ns.begin(), ns.end());
        unique(ns.begin(), ns.end());


        for (int i = 0; i < ns.size() - 1; i++) {
            string item = ns[i];
            Node t;
            t.nId = id;
            id = id + 1;
            vector<string> attr = u.findNodeAttr(item, attributes);
            for (int m = 0; m < attrName.size(); m++) {
                t.nAttribute.insert(pair<string, string>(attrName[m], attr[m]));
            }

            map<int, string> c = u.findAdjNode(item, linkages);
            t.nAdjacent = c;

            nodes.push_back(t);
        }

        NODES = nodes;
        nodeNum = nodes.size();

        return 0;
    }


    Node getNode(string attrName, string attrValue) {
        Node n;
        for (Node node : NODES) {
            if (node.getAttribute(attrName) == attrValue) {
                return node;
            }
        }


        return n;
    }

};

int main() {
    string linkageFilePath = "D:\\Networkz\\linkage.csv";
    string attributesFilePath = "D:\\Networkz\\attributes.csv";
    Network n;
    n.initializeNetwork(linkageFilePath, attributesFilePath);

//    获取网络的节点个数
    cout << "Node number:" + to_string(n.NODES.size()) << endl;
//    获取节点的属性
    for (auto node: n.NODES) {
        node.showAllAttributes();
    }
//    获取节点的邻接属性
    for (auto node: n.NODES) {
        string stkcd = node.getAttribute("stkcd");
        cout << stkcd << endl;
        node.showAllAdjacent();
    }

//  weight
    cout << n.getNode("stkcd", "1").getAdjacentAttr(2)[0] << endl;
//  type
    cout << n.getNode("stkcd", "1").getAdjacentAttr(2)[1] << endl;

    return 0;
};













