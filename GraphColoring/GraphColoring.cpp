#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

extern "C" {
    #include "ipasir.h"
}



int main(int argc, char* argv[]) {


    fstream file;
    string line;
    file.open(argv[1]);
    void* i_solver = ipasir_init();
    int edge_count = 0;
    int max_edge_count = 0;
    int prev_node1 = 0;
    string type_of_line;
    int node1, node2, node_count, colour_count;


    if (argc != 2) {
        cout << "Error received no file, Usage: GraphColoring nameOfFile.col" << endl;
        return 0;
    }

    if(file.is_open())
    {
        while (getline(file,line))
        {
            stringstream ss(line);
            stringstream parser(line);
            
            while (ss >> type_of_line >> node1 >> node2)
            {
                
                if(type_of_line.compare("e") == 0){
                    // calculate max number of edges from a single node
                    if (prev_node1 == node1){
                        edge_count++;
                    } else{
                        edge_count = 0;
                    }
                    
                    if (max_edge_count < edge_count){
                        max_edge_count = edge_count;
                        // cout << "current node: " << node1 << endl;
                        // cout << "new max : " << max_edge_count << endl;
                    }
                    prev_node1 = node1;
                }
            }
            // save the last number of node as the total number of nodes in the graph
            node_count = node1;

            // node1 -> node1 with colour0
            // node(n) -> node(n) with colour0, where n is the total number of nodes in the graph.
            // In this case n = node_count
            // node i with colour j --> node (i + n * j) 

            // Add clauses for each different node colours, so that each node gets a colour.
            // After counting the highest number of edges from one node
            // means there is at least one node that requires (max_edge_count+1) number of colours in the graph.

            colour_count = max_edge_count+ 1;
            for (size_t i = 1; i <= node_count; i++)
            {
                for (size_t j = 0; j < (colour_count); j++)
                {
                    ipasir_add(i_solver,(i + node_count * j));
                }
                ipasir_add(i_solver,0);
                
            }

            while (parser >> type_of_line >> node1 >> node2 )
            {
                // (node1)---(node2)
                // When node1 and node2 are neighbouring each other, they cannot have the same colour
                // ,thus (-node1 V -node2)
                for (size_t i = 0; i < colour_count; i++)
                {
                    ipasir_add(i_solver, -(node1 + node_count * i));
                    ipasir_add(i_solver, -(node2 + node_count * i));
                    ipasir_add(i_solver,0);
                }
                
                
            }
            
            
        }
        file.close();
    }

    int satisfiability = ipasir_solve(i_solver);
    // 10 means SATISFIABLE
    // 20 means UNSATISFIABLE
    cout << satisfiability << endl;

    if (satisfiability == 10)
    {
        cout << "c The minimum number of colours required: " << colour_count << endl;
        cout << "s SATISFIABLE " << endl;
        for (size_t i = 1; i <= node_count; i++)
        {
            for (size_t j = 0; j < (colour_count); j++)
            {
                if (ipasir_val(i_solver,(i + node_count * j)) > 0){
                    std::cout << "  Node " << i << " has colour number:  " << j << endl;
                }
            }
        }
    }else if (satisfiability == 20)
    {
        cout << "s UNSATISFIABLE " << endl;
    }else
    {
        cout << "s UNKNOWN " << endl;
    }
    
    ipasir_release(i_solver);
    return 0;
}
