# Data-communication-using-virtual-circuit-switching
implemented MPLS based virtual circuit switching in C
# Virtual circuit switching.


    			CONSOLE APPLICATION


Implementation of virtual circuit switching in network communication system.
This project covers concepts of link-disjoint routing and virtual circuit switching.

The Application will first determine two shortest cost (link-disjoint) paths for all node-pairs. You may use either hop or distance
metric. The command line parameter will specify the choice.

A connection request is Accept based on the two approach:
Optimistic Approach:
	This is used if −p command-line argument has value 0. Let Cl denote the total capacity of a link. 
	Let 
		
		bequiv = min(bmax, bavg + 0.25 ∗ (bmax − bmin )). 
		
		
A connection is admitted if the following condition is met, along each link of the path selected for connection i.

		bequiv <= (cl -  summation(bequiv)). 


where n denotes the number of existing connections sharing a given link (along the path selected for the given connection) and j denotes the index
of a connection assigned on this link.

Pessimistic Approach: 
If −p value in command-line argument is 1 then use this approach. Let Cl denote the total capacity of link l. 
A connection is admitted if the following condition is met, along each link of the path selected for connection i:

	bmax <= (cl -  summation(bmax)). 

	
where j denotes the index of a connection assigned on a given link along
the path selected for the given connection. This gets repeated for the
pessimistic approach.


HEADER FILES INCLUDED IN THE APPLICATION:

#include<bits/stdc++.h>
#include<map.h>


SOURCE FILES INCLUDED IN THE APPLICATION:

#include"floyd_warshal.cpp"


============================================================================================================================
USER DEFINED FUNCTION IMPLEMENTED IN THE APPLICATION:

FUNCTIONS USED IN MAIN FILE :

	void read_topfile(string) :-------------------------------------------------This function is used to read the topology file.
	void read_connfile(string) :------------------------------------------------This function is used to read the connection file.
	void command_line_parsing(int narg,char **arg) :----------------------------This function is used for parsing the command line input.
	void find_1st_shortest_path() :---------------------------------------------This function is used to find the first shortest paths for all node pairs, using floyd-warshall algorithm.
	unordered_multimap<int, pair<int,vector<int>>> apply_all_pair_shortest_path(int) :---This function basically takes input graph and calls floyd-warshall algorithm.
	void find_2nd_shortest_path() :---------------------------------------------This function is used to find the second link-disjoint shortest paths for all node pairs.
	void writing_path_in_routing_file() :---------------------------------------This function writes the path information in the routingtable file for all the nodes.
	void writing_path_in_forwarding_file() :------------------------------------This function writes the path information in the forwardingtable file for all established connections.
	double min_find(double, double) :-------------------------------------------This utility function is used to find the minimum of two doubles.
	void compute_bequi() :------------------------------------------------------This function is used to compute the bequi for the optimistic approach.
	bool check_path_admission(int,vector<int>,vector<vector<int>>) :------------This function is used to check whether a connection request is accepted or not.
	void path_validity_checking() :---------------------------------------------This function is used for the validation of all the connection requests.
	void path_table_computation() :---------------------------------------------This function computes the final paths table, by using path validity checking.
	void compute_forward_table_entry(int,vector<int>) :-------------------------This function computes one entry in the forwarding table.
	void forwarding_table_computation() :---------------------------------------This function is used for the computation of entire forwarding table.


FUNCTIONS USED IN FLOYD-WARSHALL FILE :

	void print_path(graph *g,int i,int j,vector<int> &v) :----------------------This function is used to print the shortest path.
	unordered_multimap<int,pair<int,vector<int>>> output_shortest_path(graph *g,unordered_multimap<int,pair<int,vector<int>>>) :- This function is used to output the shortest paths for all pairs.
	graph* input_graph1(unordered_multimap<int,vector<int>> &m,int V,int E) :---This function is used to take graph as input for the algorithm. 
	int min(int,int) :----------------------------------------------------------This utility function is used to find the minimum of two ints.
	void floyd_warshal(graph* g) :----------------------------------------------This function is basically the floyd_warshal algorithm.


----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

INPUT FORMAT OF THE APPLICATION:

	--> The application asks for topology_file, connections_file, routingtable_file, forwardingtable_file.
		If topology_file and connections_file are not specified, the program will put error "file name missing".
		If routingtable_file or forwardingtable_file are not specified, the program will create itself with names routingtablefile.txt and forwardingtablefile.txt respectively.
	--> Also, it asks the user to choose whether to use hop or distance metric(flag=1). By default, hop is used.
	--> Additionaly, user needs to enter the value of p to be 1 for pessimistic approach. By default, p is set to 0 for optimistic approach.

----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

OUTPUT FORMAT OF THE APPLICATION:

	--> The routingtablele will contain the routing table information for all the
		nodes.
				Format of the file : | Destination Node | Path | Path Delay | Path Cost |

	--> The forwardingtablefile will contain the forwarding table information
		for all the nodes.
				Format of the file : | Router's ID | ID of Incoming Port | VCID | ID of Outgoing Port | VCID |

	--> paths file will contain only one line consisting of two integers which represents the total 
		number of requested connections and the total number of admitted connections, respectively.

	--> | Connection ID | Source | Destination | Path | VCID list | Path cost |

----------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------

FAILURE CASE FOR THE APPLICATION:

	-> It takes some input files, so if these files are deleted, replaced or renamed then it will give error : PROBLEM IN OPENING FILE <filename>.


---------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------

EXECUTION STEPS OF THE APPLICATION:
	 
	 Step 1: Compile the project using g++ compiler.

	 Step 2: Run the project as given below,
	 		./a.out
			-top topology_file
			-conn connections_file 
			-rt routingtable_file
			-ft forwardingtable_file
			-path paths_file
			-flag hop|dist
			-p 0|1

		The user has the choice of selecting hop or distance.
		And, the user needs to set p to be 1 for pessimistic approach.

	EXAMPLE :
	Compile : g++ problem1.cpp

	Run : 	./a.out
			-top top14.doc
			-conn CONNNET_100.doc
			-rt routingtablefile.txt
			-ft forwardingtablefile.txt
			-path pathfile.txt
			-flag hop
			-p 1


******************************************************************EOF******************************************************************************
