using namespace std;

class Node { 
public: 
    int data; 
    Node* next; 

    Node(int x){
    	data = x;
    	next = NULL;
    }
}; 

void merge(Node* start, int size){
	cout << "merging " << size << "\n";
	int middle = size/2;
	Node* start_r = start;
	for(int i=0; i<middle; i++){
	    start_r = start_r->next;
	}
	int i = 1;
	while(i < middle){
		if(start->data <= start_r->data){
			start = start->next;
		}
		else{
			Node
		}
	}
	
};

void split(Node* start, int size)
{
	if(size > 1){
		int middle = size/2;
	    int middleright = middle;
	    if(size%2 != 0){
	    	middleright = middleright+1;
	    }

	    Node* temp = start;
	    for(int i=0; i<middle; i++){
	    	temp = temp->next;
	    }
	    //cout << "middle " << temp->data << "\n";
	    //temp is now beginning of the right side

	    split(start, middle);
	    split(temp, middleright);

	    merge(start, size);
	}


};

