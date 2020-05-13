#include <bits/stdc++.h> 
#include <chrono> 
#include <limits.h> 
using namespace std; 
using namespace std::chrono; 

// Number of cities in TSP 
#define V 5 
  
// Initial population size for the algorithm 
#define POP_SIZE 10 
  
struct individual { 
    string gnome; 
    int fitness; 
}; 
  
// Function to return a random number 
// from start and end 
int rand_num(int start, int end) 
{ 
    int r = end - start; 
    int rnum = start + rand() % r; 
    return rnum; 
} 
  
// Function to check if the character 
// has already occurred in the string 
bool repeat(string s, char ch) 
{ 
    for (int i = 0; i < s.size(); i++) { 
        if (s[i] == ch) 
            return true; 
    } 
    return false; 
} 
  
// Function to return a mutated GNOME 
// Mutated GNOME is a string 
// with a random interchange (swap mutation)
// of two genes to create variation in species 
string mutatedGene(string gnome) 
{ 
    while (true) { 
        int r = rand_num(1, V); 
        int r1 = rand_num(1, V); 
        if (r1 != r) { 
            char temp = gnome[r]; 
            gnome[r] = gnome[r1]; 
            gnome[r1] = temp; 
            break; 
        } 
    } 
    return gnome; 
} 
  
// Function to return a valid GNOME string 
// required to create the population 
string create_gnome() 
{ 
    string gnome = "0"; 
    while (true) { 
        if (gnome.size() == V) { 
            gnome += gnome[0]; 
            break; 
        } 
        int temp = rand_num(1, V); 
        if (!repeat(gnome, (char)(temp + 48))) 
            gnome += (char)(temp + 48); 
    } 
    return gnome; 
} 
  
// Function to return the fitness value of a gnome. 
// The fitness value is the path length 
// of the path represented by the GNOME. 
int cal_fitness(string gnome) 
{ 
    int map[V][V] = { { 0, 2, INT_MAX, 12, 5 }, 
                      { 2, 0, 4, 8, INT_MAX }, 
                      { INT_MAX, 4, 0, 3, 3 }, 
                      { 12, 8, 3, 0, 10 }, 
                      { 5, INT_MAX, 3, 10, 0 } }; 
    int f = 0; 
    for (int i = 0; i < gnome.size() - 1; i++) { 
        if (map[gnome[i] - 48][gnome[i + 1] - 48] == INT_MAX) 
            return INT_MAX; 
        f += map[gnome[i] - 48][gnome[i + 1] - 48]; 
    } 
    return f; 
} 
// crossover function
// crossover two parent s1,s2
string crossover(string s1,string s2)
{
    s1.erase(0,1);
	s2.erase(0,1);
	int n=s1.length();
	s1.erase(n-1);
	s2.erase(n-1);
	n--;	
	int p1=rand_num(0,n-1);
	int p2=(p1+n/2)%n;
	int i=p1;
	string s=s2;
	map<char,int> mp;
	while(i!=p2)
	{
		s[i]=s1[i];
		mp[s1[i]]++;
		i=(i+1)%n; 
	}
	int cnt=n-n/2;
	int j=p2;
	i=p2;
	while(cnt>0)
	{
		if(mp[s2[j]]==0)
		s[i]=s2[j],cnt--,i=(i+1)%n;
		j=(j+1)%n;
	}
	s='0'+s;
	s=s+'0';
	//debug(s1,s2,s,p1,p2);
	return s;
}
// Function to return the updated value 
// of the cooling element. 
int cooldown(int temp) 
{ 
    return (90 * temp) / 100; 
} 
  
// Comparator for GNOME struct. 
bool lessthan(struct individual t1, 
              struct individual t2) 
{ 
    return t1.fitness < t2.fitness; 
} 
  
// Utility function for TSP problem. 
void TSPUtil(int map[V][V]) 
{ 
    // Generation Number 
    int gen = 1; 
    // Number of Gene Iterations 
    int gen_thres = 5; 
  
    vector<struct individual> population; 
    struct individual temp; 
  
    // Populating the GNOME pool. 
    for (int i = 0; i < POP_SIZE; i++) { 
        temp.gnome = create_gnome(); 
        temp.fitness = cal_fitness(temp.gnome); 
        population.push_back(temp); 
    } 
  
    cout << "\nInitial population: " << endl 
         << "GNOME     FITNESS VALUE\n"; 
    for (int i = 0; i < POP_SIZE; i++) 
        cout << population[i].gnome << "   "
             << population[i].fitness << endl; 
    cout << "\n"; 
  
    bool found = false; 
    int temperature = 10000; 
  
    // Iteration to perform 
    // population crossing and gene mutation. 
    while (temperature > 1000 && gen <= gen_thres) { 
    
    auto start = high_resolution_clock::now(); 
    
        sort(population.begin(), population.end(), lessthan); 
        cout << "\nCurrent temp: " << temperature << "\n"; 
        vector<struct individual> new_population; 
  
        for (int i = 0; i < POP_SIZE; i++) { 
            struct individual p1 = population[i]; 
  
            while (true) { 
                string new_g = mutatedGene(p1.gnome); 
                struct individual new_gnome; 
                new_gnome.gnome = new_g; 
                new_gnome.fitness = cal_fitness(new_gnome.gnome); 
  
                if (new_gnome.fitness <= population[i].fitness) { 
                    new_population.push_back(new_gnome); 
                    break; 
                } 
                else { 
  
                    // Accepting the rejected children at 
                    // a possible probablity above threshold. 
                    float prob = pow(2.7, 
                                     -1 * ((float)(new_gnome.fitness 
                                                   - population[i].fitness) 
                                           / temperature)); 
                    if (prob > 0.5) { 
                        new_population.push_back(new_gnome); 
                        break; 
                    } 
                } 
            } 
        }
		bool vis[POP_SIZE]={false};
		int count=0;
		while(count<=POP_SIZE)
		{int p=0;
			start:
				int rand1=rand_num(0, POP_SIZE-1);
				int rand2=rand_num(0, POP_SIZE-1);
				p++;
				if(((rand1==rand2) || (!vis[rand1]) || (!vis[rand2])) && p<10)
				goto start;
				string gene=crossover(new_population[rand1].gnome,new_population[rand2].gnome);
				int f1=new_population[rand1].fitness;
				int f2=new_population[rand2].fitness;
				int f3=cal_fitness(gene);
				if(f1<f2)
				{
					if(f1<=f3 && f3<f2)
					{
						vis[rand2]=true;
						new_population[rand2].gnome=gene;
						new_population[rand2].fitness=f3;
					}
					else if(f3<f1)
					{
						vis[rand2]=true;
						new_population[rand2].gnome=new_population[rand1].gnome;
						new_population[rand2].fitness=f1;
						
						vis[rand1]=true;
						new_population[rand1].gnome=gene;
						new_population[rand1].fitness=f3;
						count++;
					}
				} else if(f2<f1){
					if(f3>=f2 && f3<f1)
					{
						vis[rand1]=true;
						new_population[rand1].gnome=gene;
						new_population[rand1].fitness=f3;
					}
					else if(f3<f2)
					{
						vis[rand1]=true;
						new_population[rand1].gnome=new_population[rand2].gnome;
						new_population[rand1].fitness=f2;
						
						vis[rand2]=true;
						new_population[rand2].gnome=gene;
						new_population[rand2].fitness=f3;count++;
					}
				}
			count++;
		}
  
        temperature = cooldown(temperature); 
        population = new_population; 
        cout << "Generation " << gen << " \n"; 
        cout << "GNOME     FITNESS VALUE\n"; 
  
        for (int i = 0; i < POP_SIZE; i++) 
            cout << population[i].gnome << "   "
                 << population[i].fitness << endl; 
        gen++; 
        
        auto stop = high_resolution_clock::now(); 
                      
                      auto duration = duration_cast<microseconds>(stop - start); 
                      cout << "Time taken by this generation: "
                     << duration.count() << " microseconds" << endl; 
    } 
} 
  
int main() 
{ 
  
    int map[V][V] = { { 0, 2, INT_MAX, 12, 5 }, 
                      { 2, 0, 4, 8, INT_MAX }, 
                      { INT_MAX, 4, 0, 3, 3 }, 
                      { 12, 8, 3, 0, 10 }, 
                      { 5, INT_MAX, 3, 10, 0 } }; 
                      auto start = high_resolution_clock::now(); 
    TSPUtil(map); 
                      auto stop = high_resolution_clock::now(); 
                      
                      auto duration = duration_cast<microseconds>(stop - start); 
                      cout<<endl<<endl<<endl;
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl; 
} 
