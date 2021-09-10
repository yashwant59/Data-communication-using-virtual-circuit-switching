 #include<bits/stdc++.h>
 #include<map>
 #include"floyd_warshal.cpp"


 using namespace std;

 FILE *rt,*ft,*pt;										
 int Nodes,Edges,nrequest;
 int metric=0,p=0;
 vector<vector<int>> link_capacity_matrix;										//vector to store link capacities
 vector<vector<int>> delay_matrix;												//vector to store propagation delays
 string top_file="",conn_file="",rt_file="",ft_file="",path_file="";

 unordered_multimap<int , vector<int> > top_map;								//map to store topology file information
 unordered_multimap<int , vector<int> > conn_map;								//map to store connection file information
 unordered_multimap<int, pair<int,vector<int>>> first_shortest_path;			//map to store first shortest path
 unordered_multimap<int, pair<int,vector<int>>> second_shortest_path;			//map to store second shortest path
 unordered_multimap<int, double> conn_to_bequi,conn_to_bmax;					//map to store bequi and bmax
 unordered_multimap<int,pair<int,vector<int>>> con_admitted_map;				//map to store accepted connection requests
 vector<vector<int>> conn_on_link;												//map to store
 unordered_multimap<int,vector<int>> vcid_list;									//map to store vcid's

 int vcid=100;
 void read_topfile(string s)											//This function is used to read the topology file.
 {
 	char st[100];
 	
 	FILE *fp=fopen(s.c_str(),"r");										//topology file is opened.
	
	if(fp!=NULL)														//if file is opened successfully
	{
		int src,dest,propogation_delay,link_capacity;
		float ignore;
 		fscanf(fp,"%d", &Nodes);										//no. of nodes
 		fscanf(fp,"%d", &Edges);										//no. of edges
		link_capacity_matrix.assign(Nodes,vector<int>(Nodes,0));							
		delay_matrix.assign(Nodes,vector<int>(Nodes,0));
		for(int i=0;i<Edges;i++)
		{
			fscanf(fp,"%d",&src);										//source node for each edge
			fscanf(fp,"%d",&dest);										//destination node for each edge
			fscanf(fp,"%d",&propogation_delay);							//propagation delay for each edge
			fscanf(fp,"%d",&link_capacity);								//link capacity for each edge
			fscanf(fp,"%f",&ignore);									//reliability for each edge (ignore it)

			link_capacity_matrix[src][dest]=link_capacity;				//stores link capacities
			link_capacity_matrix[dest][src]=link_capacity;
			delay_matrix[src][dest]=propogation_delay;					//store propagation delays
			delay_matrix[dest][src]=propogation_delay;
			vector<int> v1;
			v1.push_back(dest);
			v1.push_back(propogation_delay);
			v1.push_back(link_capacity);

			top_map.insert(make_pair(src, v1));
			
		}
		
	}
	else																//if file is not opened successfully
	{	
		cout<<"file opening error :"<<top_file;							//error in opening routingtable file
		exit(0);
	}
	fclose(fp);															//closes the routingtable file
 }

 void read_connfile(string s)											//This function is used to read the connection file.
 {
	char st[100];
 	strcpy(st, s.c_str());
 	FILE *fp=fopen(st,"r");												//connection file is opened
	
	if(fp!=NULL)														//if file is opened successfully
	{
		fscanf(fp,"%d", &nrequest);										//no. of requests
		int src,dest,bmin,bave,bmax;
		int cno=1;
		for(int i=0;i<nrequest;i++)
		{
			fscanf(fp,"%d",&src);										//source node for each request
			fscanf(fp,"%d",&dest);										//destination node for each request
			fscanf(fp,"%d",&bmin);										//minimum link capacity for each request
			fscanf(fp,"%d",&bave);										//average link capacity for each request
			fscanf(fp,"%d",&bmax);										//maximum link capacity for each request

			vector<int> v1;												//storing all of them in a vector
			v1.push_back(src);
			v1.push_back(dest);
			v1.push_back(bmin);
			v1.push_back(bave);
			v1.push_back(bmax);

			conn_map.insert(make_pair(cno, v1));
			conn_to_bmax.insert({cno,bmax});
			cno++;
		}
	}
	else																//if file is not opened successfully
	{
		cout<<"error in opening the file : "<<conn_file;				//error in opening forwardingtable file
		exit(0);
	}
	fclose(fp);															//closes the forwardingtable file
	/*
	cout<<"\n\n---------conn-file--------------\n\n";
	for(auto i=conn_map.begin();i!=conn_map.end();i++)
	{
		cout<<i->first<<" ";
		for(auto j=i->second.begin();j!=i->second.end();j++)
			cout<<*j<<" ";
		cout<<endl;
	}	
	*/
 }

void command_line_parsing(int narg,char **arg)					//This function is used for parsing the command line input.
{
 	string args[narg];

 	for(int i=0;i<narg;i++)
 		args[i]=arg[i];

 	for(int i=1;i<narg;i++)
 	{
 		if(args[i]=="-top") 								//option = -top for topology file
 		{
 			top_file=args[i+1];
 			continue;
 		}
 		if (args[i]=="-conn")								//option = -conn for connection file				
 			conn_file=args[i+1];
 		if(args[i]=="-rt")									//option = -rt for routing table file
 			rt_file=args[i+1];
 		if(args[i]=="-ft")									//option = -ft for forwarding table file
 			ft_file=args[i+1];
 		if(args[i]=="-path")								//option = -path for path file
 			path_file=args[i+1];
 		if(args[i]=="-flag")								//option = -flag to choose between hop and distance
 		{
 			if(args[i+1]=="hop")							//for flag = hop
 				metric=0;
 			else if(args[i+1]=="dist")						//for flag = dist
 				metric=1;
 		}
 		if(args[i]=="-p")									//for optimistic or pessimistic approach			
 		{
			try
			{												//p=0 for optimistic approach (default)
				p=stoi(args[i+1]);							//p=1 for pessimistic approach
			}
			catch(...)										//exception handling if it fails to convert string into integer
			{
				std::cout<<"\nInvalid argument P : not able to convert to integer\n";
				exit(0);
			}
		 }
			

 	}
 	if(top_file=="")										//exit if topology file is missing
 	{
 		cout<<"topology file name missing";
 		exit(0);
 	}
 	if(conn_file=="")										//exit if connection file is missing
 	{
 		cout<<"connection file name missing";
 		exit(0);
 	}
 	if(rt_file=="")											//creates itself if routingtable file is missing
 	{
 		rt=fopen("routingtablefile.txt","w");
 		rt_file="routingtablefile.txt";

 	}
	else													//opens routingtable file if it already exists
	{
		rt=fopen(rt_file.c_str(),"w");	 
	}
	if(path_file=="")										//create itself if path file is missing
	{
		pt=fopen("path_file.txt","w");
 		path_file="path_file.txt";
	}
	else													//opens path file if it already exists
	{
		pt=fopen(path_file.c_str(),"w");
	}
	
	 
 	if(ft_file=="")											//create itself if forwardingtable file is missing
 	{
 		ft= fopen("forwardingtablefile.txt","w");
 		ft_file="forwardingtablefile.txt";
 	}
	else													//opens forwardingtable file if it already exists
	{
		ft= fopen(ft_file.c_str(),"w");
	}
	 

 	read_topfile(top_file);
	read_connfile(conn_file);
}

void find_1st_shortest_path()									//This function is used to find the first shortest paths for all node pairs, using floyd-warshall algorithm.
{
	graph *g=input_graph1(top_map,Nodes,Edges,metric);
	floyd_warshal(g);											//call to floyd warshall algorithm
	first_shortest_path=output_shortest_path(g,first_shortest_path);

	/*cout<<"-----------first shortest path-----------\n";
	for(auto i=first_shortest_path.begin();i!=first_shortest_path.end();i++)
	{
		cout<<i->first<<" "<<i->second.first<<" ";
		for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
		{
			cout<<*j<<" ";
		}
		cout<<endl;
	}*/
}

unordered_multimap<int, pair<int,vector<int>>> apply_all_pair_shortest_path(int no_of_link_removed)			//This function basically takes input graph and calls floyd-warshall algorithm.
{
	unordered_multimap<int, pair<int,vector<int>>> intermediate_shortest_path;
	graph *g=input_graph1(top_map,Nodes,Edges-no_of_link_removed,metric);
	floyd_warshal(g);																				//call to floyd warshall algorithm
	intermediate_shortest_path=output_shortest_path(g,intermediate_shortest_path);

	/*cout<<"-----------second shortest path-----------\n";
	for(auto i=intermediate_shortest_path.begin();i!=intermediate_shortest_path.end();i++)
	{
		cout<<i->first<<" "<<i->second.first<<" ";
		for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
		{
			cout<<*j<<" ";
		}
		cout<<endl;
	}*/

	return intermediate_shortest_path;
}

void find_2nd_shortest_path()									//This function is used to find the second link-disjoint shortest paths for all node pairs.
{

	vector<vector<int>> topology_path_marker;
	for(int i=0;i<Nodes;i++)									//creates a mapping of path from source node and destination node.
	{
		vector<int> v;
		for(int j=0;j<Nodes;j++)
		{
			v.push_back(0);
		}
		topology_path_marker.push_back(v);
	}

	for(auto i=first_shortest_path.begin();i!=first_shortest_path.end();i++)			//for deleting the edges to find 2nd shortest paths
	{
		unordered_multimap<int , vector<int> > top_map_link_removed;
		
		int no_of_link_removed=0;											

		if(topology_path_marker[i->first][i->second.first]==1)
		{
			continue;
		}

		if(i->second.second.size()==0)									//if no path exists, then continue on the next path
				continue;
		
		for(auto j=i->second.second.begin();j!=(i->second.second.end()-1);j++)
		{
			unordered_multimap<int , vector<int> > map_to_find_link;
			int second_value_flag=0;
			auto it=top_map.find(*j);
			if(it==top_map.end())
			{
				it=top_map.find(*(j+1));

				if(it==top_map.end())
					continue;
				second_value_flag=1;
			}
			while(true)
			{
				
				bool condition_check=false;
				if(second_value_flag==1)
				{
					condition_check=(*(it->second.begin())==*j)?true:false;
				}
				else
				{
					condition_check=(*(it->second.begin())==*(j+1))?true:false;
				}
				second_value_flag=0;
				if(condition_check)						
				{

					top_map_link_removed.insert(make_pair(it->first,it->second));			//inserting the links removed from top map
					no_of_link_removed++;
					top_map.erase(it);
					break;
				}
				map_to_find_link.insert(make_pair(it->first,it->second));
				top_map.erase(it);
				it=top_map.find(*j);
				if(it==top_map.end())
				{
					it=top_map.find(*(j+1));

					if(it==top_map.end())
						continue;
					second_value_flag=1;
				}
			}
			for(auto i=map_to_find_link.begin();i!=map_to_find_link.end();i++)
				top_map.insert(make_pair(i->first,i->second));
			
			map_to_find_link.clear();
			//delete &map_to_find_link;

		}

			auto temp_map=apply_all_pair_shortest_path(no_of_link_removed);

			auto temp_it=temp_map.find(i->first);
			while(true)
			{
				
				if(temp_it->second.first==i->second.first)						//if error comes then check here
				{
					second_shortest_path.insert(make_pair(temp_it->first,temp_it->second));
					reverse(temp_it->second.second.begin(),temp_it->second.second.end());
					second_shortest_path.insert(make_pair(temp_it->second.first,make_pair(temp_it->first,temp_it->second.second)));
					
					break;
				}
				
				temp_map.erase(temp_it);
				temp_it=temp_map.find(i->first);
			}
			temp_map.clear();
			//delete &temp_map;



			for(auto i=top_map_link_removed.begin();i!=top_map_link_removed.end();i++)		//adding the removed paths back again to topology map
			{
				top_map.insert(make_pair(i->first,i->second));
			}


			topology_path_marker[i->first][i->second.first]=1;
			topology_path_marker[i->second.first][i->first]=1;


	top_map_link_removed.clear();

	//delete &top_map_link_removed;
		
	}


/*	cout<<"-----------second shortest path-----------\n";
	for(auto i=second_shortest_path.begin();i!=second_shortest_path.end();i++)
	{
		cout<<i->first<<" "<<i->second.first<<" ";
		for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
		{
			cout<<*j<<" ";
		}
		cout<<endl;
	}*/


}

void writing_path_in_routing_file()										//This function writes the path information in the routingtable file for all the nodes.
{
	FILE *fp=fopen("routingtablefile.txt","w");							//opens routingtable file in write mode
	if(fp!=NULL)														//if it gets opened successfully
	{
		fprintf(fp,"-----------------First Shortest path :---------------- \n\n");
		fprintf(fp,"Src Node\t|\tDest Node\t|\tPath");
		for(int i=0; i<7; i++)
			fprintf(fp, "\t");
		fprintf(fp, "|\tDelay\t|\tCost\t|\n");
		for(auto i=first_shortest_path.begin();i!=first_shortest_path.end();i++)
		{
			if(i->first==i->second.first) continue;
			fprintf(fp,"|%d\t\t| ",i->first);								//prints source node in the file
			fprintf(fp,"\t%d\t\t| ",i->second.first);						//prints destination node in the file
			int count=0,delay=0;
			for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
			{
				fprintf(fp,"\t%d ",*j);										//prints the path in the file
				if(*j==*i->second.second.rbegin())
					continue;
				
				if((j+1)!=i->second.second.end())
					delay+=delay_matrix[*j][*(j+1)];						//computes delay for each path from source to destination node
				count++;
			}
			for(int i=count;i<=6;i++)
				fprintf(fp,"\t");
			fprintf(fp,"|\t");
			fprintf(fp,"%d\t",delay);										//prints path delay in the file
			fprintf(fp,"|\t%ld\t|\n",(i->second.second.size()-1));

		}

		//FOR SECOND SHORTEST PATH


		fprintf(fp,"\n\n---------------Second Shortest paths-------------------\n\n");
		fprintf(fp,"Src Node\t|\tDest Node\t|\tPath");
		for(int i=0; i<6; i++)
			fprintf(fp, "\t");
		fprintf(fp, "|\tDelay\t|\tCost\t|\n");
		for(auto i=second_shortest_path.begin();i!=second_shortest_path.end();i++)
		{
			if(i->first==i->second.first) continue;
			fprintf(fp,"|%d\t\t| ",i->first);							//prints source node in the file
			fprintf(fp,"\t%d\t\t| ",i->second.first);					//prints destination node in the file
			int count=0,delay=0;
			for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
			{
				fprintf(fp,"\t%d ",*j);									//prints the path in the file
				if(*j==*i->second.second.rbegin())
					continue;
				
				if((j+1)!=i->second.second.end())
					delay+=delay_matrix[*j][*(j+1)];					//computes delay for each path from source to destination node
				count++;
			}
			
			
			for(int i=count;i<=6;i++)
				fprintf(fp,"\t");
			fprintf(fp,"|\t");
			fprintf(fp,"%d\t",delay);									//prints path delay in the file
			fprintf(fp,"|\t%ld\t|\n",(i->second.second.size()-1));

			
		}
	}
	else																//if file fails to get opened 
	{
		cout<<"error opening routing file";								//error in opening the routing table file
		exit(0);
	}
}

double min_find(double a, double b)										//This utility function is used to find the minimum of two doubles.
{
	if(a>b)			
		return b;
	else
		return a;
	
}

void compute_bequi()													//This function is used to compute the bequi for the optimistic approach.
{
	double bequivalent=0;

	int index=1;
	auto it=conn_map.find(index);
	while(it!=conn_map.end())											//computing bequi for all the connection requests
	{
		bequivalent=min_find(it->second[4],(it->second[3]+(0.25*(it->second[4]-it->second[2]))));

		conn_to_bequi.insert(make_pair(index,bequivalent));					
		index++;
		it=conn_map.find(index);
		
	}
	
}

//This function is used to check whether a connection request is accepted or not.
bool check_path_admission(int connection_number,vector<int> &path_vector,vector<vector<int>> &link_mapping)			
{
	auto map_it=conn_to_bequi;
	if(p==1)
		map_it=conn_to_bmax;
	if(path_vector.size()==0)
		return false;
	int check_bit=-1;
	for(auto i=path_vector.begin();i!=path_vector.end();i++)				//checking whether a path is following the given condition to be admitted
	{
		
		if(*i==*path_vector.rbegin())
			continue;
		check_bit=1;
		double sum=0;
		for(auto j=conn_on_link[link_mapping[*i][*(i+1)]].begin();j!=conn_on_link[link_mapping[*i][*(i+1)]].end();j++)
		{
			sum+=map_it.find(*j)->second;
		}

		if(map_it.find(connection_number)->second<=(link_capacity_matrix[*i][*(i+1)]-sum))
		{
			check_bit=1;
		}
		else
		{
			check_bit=0;
			break;
		}

	}

	if(check_bit==1)								//if connection request is accepted then add it to conn_on_link map 
	{
		for(auto i=path_vector.begin();i!=path_vector.end();i++)
		{
			if(*i==*path_vector.rbegin())
				continue;
			
			conn_on_link[link_mapping[*i][*(i+1)]].push_back(connection_number);
			
		}

		return true;							//if connection is accepted, then it returns true
	}
	else
	{
		return false;							//if connection is not accepted, then it returns false
	}
	
}

void path_validity_checking()											//This function is used for the validation of all the connetion requests.
{
	if(p==0)															//p=0 for optimistic approach
	{
		compute_bequi();												//computes the bequi for the optimistic approach
		/*cout<<"\n------------bEquivalent-------------\n\n";
		for(auto i=conn_to_bequi.begin();i!=conn_to_bequi.end();i++)
		{
			cout<<i->first<<" "<<i->second;
			cout<<"\n";
		}*/
	}

	
	vector<vector<int>> link_mapping;
	link_mapping.assign(Nodes,vector<int>(Nodes,0));
	int map_index=0;
	
	for(auto i=top_map.begin();i!=top_map.end();i++)					//creates a mapping for a link to no. of connections
	{
		vector<int> v1;
		conn_on_link.push_back(v1);
		link_mapping[i->first][i->second[0]]=map_index;
		link_mapping[i->second[0]][i->first]=map_index;
		map_index++;
	}

	
	//int mapping[Nodes][Nodes]={0};
	vector<vector<int>> mapping;
	mapping.assign(Nodes,vector<int>(Nodes,0));
	map_index=0;
	vector<vector<int>> mapped_path_vector;
	for(auto i=first_shortest_path.begin();i!=first_shortest_path.end();i++)			//loop for information of first shortest paths
	{

		mapping[i->first][i->second.first]=map_index;
		mapped_path_vector.push_back(i->second.second);
		map_index++;
	}
	
	vector<vector<int>> mapping_2nd;
	mapping_2nd.assign(Nodes,vector<int>(Nodes,0));
	map_index=0;
	vector<vector<int>> mapped_path_vector_2nd;
	for(auto i=second_shortest_path.begin();i!=second_shortest_path.end();i++)			//loop for information of second shortest paths
	{

		mapping_2nd[i->first][i->second.first]=map_index;
		mapped_path_vector_2nd.push_back(i->second.second);
		map_index++;
	}
	int count=1;
	for(auto i=conn_map.find(count);i!=conn_map.end();i=conn_map.find(++count))
	{
		
		bool check=check_path_admission(i->first,mapped_path_vector[mapping[i->second[0]][i->second[1]]],link_mapping);

		if(check)
		{
			con_admitted_map.insert({i->first,{1,mapped_path_vector[mapping[i->second[0]][i->second[1]]]}});
		}
		else 
		{
			bool check2=check_path_admission(i->first,mapped_path_vector_2nd[mapping_2nd[i->second[0]][i->second[1]]],link_mapping);
			if(check2)
				con_admitted_map.insert({i->first,{1,mapped_path_vector_2nd[mapping_2nd[i->second[0]][i->second[1]]]}});
			else
				con_admitted_map.insert({i->first,{0,mapped_path_vector_2nd[mapping_2nd[i->second[0]][i->second[1]]]}});	
		}
	}
	mapping.clear();
	mapping_2nd.clear();
	mapped_path_vector.clear();
	mapped_path_vector_2nd.clear();

	//cout<<"\n\n------------connection admission flags-----------\n\n";
	count=1;
	int no_connection_admitted=0;
	for(auto i=con_admitted_map.find(count);i!=con_admitted_map.end();i=con_admitted_map.find(++count))				//finds the no. of connection requests accepted
	{
		
		if(i->second.first==1)
			no_connection_admitted++;					
		else
		{
			continue;
		}
		//cout<<i->first<<" "<<i->second.first;
	/*	for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
		{
	//		cout<<" "<<*j;
		}
		cout<<endl;*/
	}

	cout<<"\n\nNumber of requested connections : "<<nrequest<<endl;
	cout<<"Number of Accepted connections : "<<no_connection_admitted<<endl;

	fprintf(pt,"%s","Number of requested connections : ");
	fprintf(pt,"%d\n",nrequest);
	fprintf(pt,"%s","Number of Accepted connections : ");
	fprintf(pt,"%d\n",no_connection_admitted);
}

void path_table_computation()												//This function computes the final paths table, by using path validity checking.
{
	path_validity_checking();												//validates all the connection requests.
}

//This function computes one entry in the forwarding table.
void compute_forward_table_entry(int connection_number,vector<int> &path_vector,vector<vector<pair<int,int>>> &port,int *port_no,vector<vector<vector<int>>> &table,int *table_no)		
{
	if(path_vector.size()==0)
		return;
	
	vector<int> con_vcid;
	int last_vcid=0;
	vector<int> src_entry(4,0);
	for(auto i=path_vector.begin();i!=path_vector.end();i++)				
	{
		if(*i==*path_vector.rbegin())
			continue;
		
		vector<int> dest_entry(4,0);
		
		if(*i==*path_vector.begin())				//assigning vcid and port no. to be -1, if start node is source node
		{
			src_entry[0]=-1;
			src_entry[1]=-1;
			src_entry[3]=vcid++;
			con_vcid.push_back(src_entry[3]);
			last_vcid=src_entry[3];	
		}
		if(*(i+1)==*path_vector.rbegin())				//assigning vcid and port no. to be -1, if end node is destination node
		{
			dest_entry[2]=-1;
			dest_entry[3]=-1;
		}
		if(src_entry[2]==0)
		{	
			if(port[*i][*(i+1)].first==-1)				//assigning vcid and port no. by some unique no. to internal node of the connection
			{
				src_entry[2]=port_no[*i]++;
				port[*i][*(i+1)].first=src_entry[2];
				port[*(i+1)][*i].second=src_entry[2];
			}
			else
			{
				src_entry[2]=port[*i][*(i+1)].first;
			}
		}
			

		if(dest_entry[1]==0)
			dest_entry[1]=last_vcid;
		
		if(dest_entry[3]==0)
		{	
			dest_entry[3]=vcid;
			vcid++;
			con_vcid.push_back(dest_entry[3]);
		}
		if(dest_entry[0]==0)
		{
			if(port[*i][*(i+1)].second==-1)
			{
				dest_entry[0]=port_no[*(i+1)]++;
				port[*i][*(i+1)].second=dest_entry[0];
				port[*(i+1)][*i].first=dest_entry[0];
			}
			else
			{
				dest_entry[0]=port[*i][*(i+1)].second;
			}
		}
		
		last_vcid=dest_entry[3];
	//	auto node_ptr=forwarding_table.find(*i);
	
		table[*i][table_no[*i]][0]=src_entry[0];					//storing these entries in the forwarding table
		table[*i][table_no[*i]][1]=src_entry[1];
		table[*i][table_no[*i]][2]=src_entry[2];
		table[*i][table_no[*i]][3]=src_entry[3];
		table_no[*i]++;
	/*	if(node_ptr==forwarding_table.end())
		{
			vector<vector<int>> v;
			v.push_back(src_entry);
			forwarding_table.insert(make_pair(*i,v));
		}
		else
		{
			node_ptr->second.push_back(src_entry);
		}
		*/
		src_entry=dest_entry;

		if(*(i+1)==*path_vector.rbegin())
		{
		
			table[*(i+1)][table_no[*(i+1)]][0]=src_entry[0];			//storing these entries in the forwarding table
			table[*(i+1)][table_no[*(i+1)]][1]=src_entry[1];
			table[*(i+1)][table_no[*(i+1)]][2]=src_entry[2];
			table[*(i+1)][table_no[*(i+1)]][3]=src_entry[3];
			table_no[*(i+1)]++;
			/*auto node_ptr=forwarding_table.find(*(i+1));
			if(node_ptr==forwarding_table.end())
			{
				vector<vector<int>> v;
				v.push_back(src_entry);
				forwarding_table.insert(make_pair(*i,v));
			}
			else
			{
				node_ptr->second.push_back(src_entry);
			}*/
		}
		
		
	}
	vcid_list.insert(make_pair(connection_number,con_vcid));
}

void forwarding_table_computation()													//This function is used for the computation of entire forwarding table.
{
	vector<vector<pair<int,int>>> port;
	vector<vector<vector<int>>> table;
	int table_no[Nodes]={0};
	table.assign(Nodes,vector<vector<int>>(nrequest,vector<int>(4,0)));
	int port_no[Nodes]={1},index=1;
	port.assign(Nodes,vector<pair<int,int>>(Nodes,{-1,-1}));
	for(auto i=con_admitted_map.find(index);i!=con_admitted_map.end();i=i=con_admitted_map.find(++index))			//for computing an entry of forwarding table
	{
		if(i->second.first==0)
			continue;

		compute_forward_table_entry(i->first,i->second.second,port,port_no,table,table_no);
	}

	port.clear();
	

	index=0;
/*	for(auto itr=forwarding_table.begin();itr!=forwarding_table.end();itr++)
	{
		auto i=itr->second;
		cout<<"\n\n\n---------forward table entry of node "<<itr->first<<" ---------\n\n";
		for(auto j=i.begin();j!=i.end();j++)
		{
			for(auto k=j->begin();k!=j->end();k++)
			{
				cout<<*k<<" ";
			}
			cout<<endl;
		}
	}*/
	fprintf(ft,"%s","Router Id\tIncoming Port\tIncoming vcid\toutgoing port\toutgoing vcid\n\n");
	for(int i=0;i<Nodes;i++)
	{
		//cout<<"\n\n--------forward table for node "<<i<<"------\n\n";
		for(int j=0;j<table_no[i];j++)
		{
			//writes the entries in the forwarding table file for all the nodes and all the connections
			fprintf(ft,"%d\t\t",i);
			fprintf(ft,"%d\t\t",table[i][j][0]);				
			fprintf(ft,"%d\t\t",table[i][j][1]);
			fprintf(ft,"%d\t\t",table[i][j][2]);
			fprintf(ft,"%d\n",table[i][j][3]);
		//	cout<<endl;
		}

		fprintf(ft,"\n\n");
	}
	index=0;
/*	cout<<"\n\n-------------------vcid list-----------\n\n";
	for(auto i=vcid_list.begin();i!=vcid_list.end();i++)
	{
		cout<<i->first<<"  ";
		for(auto j=i->second.begin();j!=i->second.end();j++)
		{
			cout<<*j<<" ";
		}
		cout<<endl;
	}
*/
	
}

 int main(int narg,char **arg)
 {
 	if(narg<5)											//atleast 5 arguments have to be specified otherwise it will exit
 	{
 		cout<<"argument missing!";						//-top and -conn are mandatory, others are not.
 		exit(0);
 	}

	command_line_parsing(narg,arg);						//function call for command line parsing
	find_1st_shortest_path();							//finds first shortest paths
	
	find_2nd_shortest_path();							//finds second shortest paths
	writing_path_in_routing_file();						//writes all the paths' information in routing table file

	path_table_computation();							//computation of path table
	forwarding_table_computation();						//computation of entire forwarding table
	//writing_path_in_forwarding_file();				//writes all the paths' information in forwarding table file


	int index=1;
	for(int tep=0;tep<76;tep++)
		cout<<"==";
	cout<<"\nconnection no   |    source     |  destination  |\tpath\t\t\t\t\t|\tVCID list\t\t\t| path cost\t|\n";
	for(int tep=0;tep<76;tep++)
		cout<<"==";

	cout<<"\n";
	for(auto i=con_admitted_map.find(index);i!=con_admitted_map.end();i=con_admitted_map.find(++index))
	{
		if(i->second.first==0)
			continue;
		cout<<"\t"<<index<<"\t|";
		cout<<"\t"<<*i->second.second.begin()<<"\t|";
		cout<<"\t"<<*i->second.second.rbegin()<<"\t|  ";
		for(auto j=i->second.second.begin();j!=i->second.second.end();j++)
		{
			cout<<*j<<"\t";
		}
		for(int k=i->second.second.size();k<6;k++)
		{
			cout<<"\t";
		}
		cout<<"|  ";
		auto vc_list=vcid_list.find(index);
		if(vc_list!=vcid_list.end())
		{	
			for(auto j=vc_list->second.begin();j!=vc_list->second.end();j++)
			{
				cout<<*j<<"\t";
			}
			for(int k=vc_list->second.size();k<5;k++)
				cout<<"\t";
		}
		cout<<"|\t"<<i->second.second.size()-1<<"\t|\n";

	}

	fclose(ft);											//closes forwarding table file
	fclose(rt);											//closes routing table file
	fclose(pt);											//closes path file
	
 	return 0;
 }