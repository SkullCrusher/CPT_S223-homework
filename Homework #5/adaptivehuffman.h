#ifndef __ADAPTIVEHUFFMAN_H__
#define __ADAPTIVEHUFFMAN_H__

/* Written by David Harkins. */

#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

class AdaptiveHuffman{

	struct Node{
		
		int Weight;

			// The character the node aligns to.
		char Letter;
		
			// Used in the zero node to contain all of the other alphabet.
		std::string Character_Set;

		Node* Parent;

		Node* Left_Child;
		Node* Right_Child;

		Node* Next;
		Node* Previous;

			//Default constructor.
		Node(){
			Weight = 0;
			Letter = -1;

			Character_Set = "";
			
			Parent = NULL;
			Left_Child = NULL;
			Right_Child = NULL;

			Next = NULL;
			Previous = NULL;
		}

		void DeleteChildren(){

		}
	};

		//Save a local copy of the alphabet.
	private: string * Alphabet;

		// Not really used but is a nice feature.
	private: Node CopyNode(Node* argument){
		Node Temp;

		Temp.Character_Set = argument->Character_Set;
		Temp.Left_Child = argument->Left_Child;
		Temp.Letter = argument->Letter;
		Temp.Next = argument->Next;
		Temp.Parent = argument->Parent;
		Temp.Previous = argument->Previous;
		Temp.Right_Child = argument->Right_Child;
		Temp.Weight = argument->Weight;

		return Temp;
	}


		

		// Swap two nodes only next and prev and fix the parents reference.
	private: void Handle_Node_Switch(Node* NodeA, Node* NodeB){

			// All of the data that is required.
		Node* Parent_Previous = NULL;
		Node* NodeB_Next = NodeB->Next;
		Node* NodeB_Previous = NodeB->Previous;
		Node* NodeA_Next = NodeA->Next;
		Node* NodeA_Previous = NodeA->Previous;

		Node* BeforeNodeA_Next = NULL;

			// Check if there is a parent to NodeB if so change it's previous
		if (NodeB->Next != NULL){
			NodeB->Next->Previous = NodeA;
		}
			
			// Assign next to the parent.
		NodeA->Next = NodeB_Next;

			// Set the one before a to b.
		if (NodeA->Previous != NULL){
			NodeA->Previous = NodeB;
		}

			// Set B previous to the correct.
		NodeB->Previous = NodeA_Previous;

			// Swap A and B
		NodeA->Previous = NodeB;
		NodeB->Next = NodeA;
	
		std::string NodeA_Is_rightorleft = "";
		

			// Fix the parents A
		if (NodeA->Parent != NULL){
			// Find if NodeA is the left or right child.
			if (NodeA->Parent->Left_Child == NodeA){
				//NodeA->Parent->Left_Child = NodeB;
				NodeA_Is_rightorleft = "Left";
			}else{
				//NodeA->Parent->Right_Child = NodeB;
				NodeA_Is_rightorleft = "Right";
			}
		}

		std::string NodeB_Is_rightorleft = "";

			// Fix the parents B
		if (NodeB->Parent != NULL){
				// Find if NodeB is the left or right child.
			if (NodeB->Parent->Left_Child == NodeB){
				NodeB_Is_rightorleft = "Left";
			}else{
				NodeB_Is_rightorleft = "Right";
			}
		}

			// Actually change the parents based off where we found the children.
		if (NodeA_Is_rightorleft == "Right"){ NodeA->Parent->Right_Child = NodeB;}
		if (NodeA_Is_rightorleft == "Left"){ NodeA->Parent->Left_Child = NodeB; }

		if (NodeB_Is_rightorleft == "Right"){ NodeB->Parent->Right_Child = NodeA; }
		if (NodeB_Is_rightorleft == "Left"){ NodeB->Parent->Left_Child = NodeA; }

		// Swap the parents.
		Node * TempParent = NodeA->Parent;
		NodeA->Parent = NodeB->Parent;
		NodeB->Parent = TempParent;
			
	}

		// Required for the jagermeister to function.
	private: void PepsiTracer(Node* Current, int level, std::vector<Node *> &Dump_Nodes,std::vector<int> &Dump_Level){

		Dump_Nodes.push_back(Current);

		Dump_Level.push_back(level);

		if (Current->Left_Child != NULL){
			PepsiTracer(Current->Left_Child, (level + 1), Dump_Nodes, Dump_Level);
		}
				
		if (Current->Right_Child != NULL){
			PepsiTracer(Current->Right_Child, (level + 1), Dump_Nodes, Dump_Level);
		}

		
	}

		// Remove the problems with sweet goodness.
	private: void TheJagermeisterSolution(Node* Root){

		std::vector<Node *> Dump_Nodes;
		std::vector<int> Dump_Level;

		PepsiTracer(Root, 0, Dump_Nodes, Dump_Level);

		if (Dump_Nodes.size() < 2){
			return;
		}

		bool change = true;

		while (change){
			change = false;
			for (unsigned int i = 0; i < Dump_Nodes.size() - 1; i++){

				if (Dump_Level[i] > Dump_Level[i + 1]){
					change = true;
					Node* LocalTemp = Dump_Nodes[i];
					Dump_Nodes[i] = Dump_Nodes[i + 1];
					Dump_Nodes[i + 1] = LocalTemp;

					int LocalTemp2 = Dump_Level[i];
					Dump_Level[i] = Dump_Level[i + 1];
					Dump_Level[i + 1] = LocalTemp2;
				}
			}
		}

		change = true;

		while (change){
			change = false;
			for (unsigned int i = 0; i < Dump_Nodes.size() - 1; i++){
				
				if (Dump_Nodes[i]->Weight > Dump_Nodes[i + 1]->Weight && Dump_Level[i] == Dump_Level[i + 1]){
					change = true;
					Node* LocalTemp = Dump_Nodes[i];
					Dump_Nodes[i] = Dump_Nodes[i + 1];
					Dump_Nodes[i + 1] = LocalTemp;

					int LocalTemp2 = Dump_Level[i];
					Dump_Level[i] = Dump_Level[i + 1];
					Dump_Level[i + 1] = LocalTemp2;
				}
			}
		}

		Dump_Nodes[0]->Next = NULL;
		Dump_Nodes[0]->Previous = Dump_Nodes[1];


		for (unsigned int i = 1; i < Dump_Nodes.size() - 1; i++){

			Dump_Nodes[i]->Previous = Dump_Nodes[i + 1];
			Dump_Nodes[i]->Next = Dump_Nodes[i - 1];
		}

		Dump_Nodes[Dump_Nodes.size() - 1]->Next = Dump_Nodes[Dump_Nodes.size() - 2];
		Dump_Nodes[Dump_Nodes.size() - 1]->Previous = NULL;

	}

		// Handles when a character is increased.
	private: void Adjust_Character_Weight_Huffman(Node* Current_Node){
		
		Node* Temp = Current_Node;
		while (Temp->Parent != NULL){
			Temp = Temp->Parent;
		}

			// If the node is a character we only care about their parents.
		if (Current_Node->Letter != -1){
			Current_Node->Weight++;
			Current_Node = Current_Node->Next;
		}

			// Solves any problems in life, I personally guarantee it.
		TheJagermeisterSolution(Temp);
		
		do{

			// Increase their weight.
			Current_Node->Weight++;

			// Simple case, we are root.
			if (Current_Node->Parent == NULL){
				return;
			}

			if (Current_Node->Next != NULL){

				// Check if the current weight is more then the next.
				if (Current_Node->Weight > Current_Node->Next->Weight){
					// Our case is unbalanced so we need to swap.

					Node * SwapWith = Current_Node;

					// We need to find the leader of the next weight.
					while (SwapWith->Next != NULL){
						SwapWith = SwapWith->Next;

						if (Current_Node->Weight <= SwapWith->Weight){
							SwapWith = SwapWith->Previous;
							break;
						}
					}
					// Use the funtion to swap the nodes.
					Handle_Node_Switch(Current_Node, SwapWith);
				}
			}

			//Continue looping until we try to go past the root.
			Current_Node = Current_Node->Parent;

		} while (Current_Node != NULL);
	}

		// Used by the zero node to insert new characters.
	private: void Insert_Character_Into_Huffman(Node* &Current_Node, char Character){

		Node * NewObject = new Node();
		Node * NewCharacter = new Node();

		NewObject->Weight = 1;
		NewObject->Parent = Current_Node->Parent;
		NewObject->Right_Child = NewCharacter;
		NewObject->Left_Child = Current_Node;

		NewCharacter->Weight = 1;
		NewCharacter->Parent = NewObject;
		NewCharacter->Letter = Character;

		std::string LeftorRight = "";

			// Fix the parent's child if current has a parent.
		if (Current_Node->Parent != NULL){
			if (Current_Node->Parent->Left_Child == Current_Node){
				Current_Node->Parent->Left_Child = NewObject;

				LeftorRight = "Left";
			}else{
				Current_Node->Parent->Right_Child = NewObject;

				LeftorRight = "Right";
			}
		}

		Current_Node->Parent = NewObject;

			// Swap them.		
		Node * Temp = Current_Node;
		Current_Node = NewObject;

			// Set the next and previous.
		Current_Node->Previous = NewCharacter;
		NewCharacter->Previous = Temp;
		NewCharacter->Next = Current_Node;
		Temp->Next = NewCharacter;

		// Set the next and previous for the parent of the newly inserted object.
			// if the node is a left child then it connects to the right child else it connects to the parent.
		if (LeftorRight == "Left"){

			//if the parent does not have a right child we convert the case to a right case.
			if (Current_Node->Parent->Right_Child == NULL){
				LeftorRight = "Right";
			}else{
				Current_Node->Parent->Right_Child->Previous = Current_Node;
				Current_Node->Next = Current_Node->Parent->Right_Child;
			}
		}

			//Simple case.
		if (LeftorRight == "Right"){
			Current_Node->Parent->Previous = Current_Node;
			Current_Node->Next = Current_Node->Parent;
		}
	}

		// Search the tree looking for the character.
	private: int Encode_Character(char CurrentChar, std::string &result, Node* &Root){

		bool Found = false;

		Node* Search_Results = Root;
			// Find the node with the chacter.

		while (Found == false){

				// Look at the current node and see if it has the character.
			if (Search_Results->Letter == CurrentChar){
				
				// Check if there are any off balance points.
				Adjust_Character_Weight_Huffman(Search_Results);

					// We are finished so return.
				return 0;
			}

				// Check to see if it's the zero node.
			if (Search_Results->Character_Set.size() > 0){
					// There is a default of 1 at the start of the list so add one.
					result += "1";

					// Search current node for the character
				for (unsigned int i = 0; i < Search_Results->Character_Set.size(); i++){
					
						// If that character is the one we are looking for output 0 else 1.
					if (Search_Results->Character_Set[i] == CurrentChar){

						result += "0";						
						Found = true;

							// Insert the character in the huffman.
						Insert_Character_Into_Huffman(Search_Results, CurrentChar);
							
						Root = Search_Results;

						while (Root->Parent != NULL){
							Root = Root->Parent;
						}



						Search_Results = Search_Results->Left_Child;

							// Replace the character with the last one.
						if (Search_Results->Character_Set[i] > 1){

							unsigned int size = Search_Results->Character_Set.size() - 1;

							Search_Results->Character_Set[i] = Search_Results->Character_Set[size];
							//Search_Results->Character_Set.pop_back(); // Delete the last one.
							Search_Results->Character_Set.erase(Search_Results->Character_Set.size() - 1, Search_Results->Character_Set.size() - 1);
						}else{
								// The string should be empty so empty it.
							Search_Results->Character_Set = "";
						}

							// Change the weight of the parent if it exists.
						if (Search_Results->Parent != NULL){	

							Search_Results = Search_Results->Parent;
							if (Search_Results->Parent != NULL){

								Adjust_Character_Weight_Huffman(Search_Results->Parent);

							}
						}

							// Found the character so return.
						return 0;
					}else{
						result += "1";
					}	
				}
			}
			
				// Which direction should the function follow if the children don't have the character.
			std::string LeftorRight;
			
				// Check if there is a left child.
			if (Search_Results->Left_Child != NULL){
				if (Search_Results->Left_Child->Letter == -1){
					LeftorRight = "Left";
				}else if (Search_Results->Left_Child->Letter == CurrentChar){

					// The left child happens to be the character so jump out with it.
					result += "0";

					Search_Results = Search_Results->Left_Child;

					// Add one to the count of that letter.
					//Search_Results->Weight++; //moved to Adjust character weight

					// Check if there are any off balance points.
					Adjust_Character_Weight_Huffman(Search_Results);

					// We are finished so return.
					return 0;
				}
			}

				// Check if there is a right child.
			if (Search_Results->Right_Child != NULL){
				if (Search_Results->Right_Child->Letter == -1){
					LeftorRight = "Right";
				}else if (Search_Results->Right_Child->Letter == CurrentChar){

					// The right child happens to be the character so jump out with it.
					result += "1";

					Search_Results = Search_Results->Right_Child;

					// Add one to the count of that letter.
					//Search_Results->Weight++; //moved to Adjust character weight

					// Check if there are any off balance points.
					Adjust_Character_Weight_Huffman(Search_Results);

					// We are finished so return.
					return 0;
				}
			}

				// The character was not in ether the left or right so we jump to the next node.
			if (LeftorRight == "Left"){
				result += "0";
				Search_Results = Search_Results->Left_Child;

			}else{
				result += "1";
				Search_Results = Search_Results->Right_Child;
			}
			
		}
		
		return 1;
	}

		// Encodes an entire string by using Encode_Character.
	public:	int encode(std::string *msg, std::string &result, int rbuff_size){

			//The root of the huffman tree.
		Node* Root = NULL;

			//Create a zero node and put it into the tree.
		Node Zero_Node;
		Zero_Node.Character_Set = *Alphabet;

		Root = &Zero_Node;

			// Loop through all of the characters.
		for (unsigned i = 0; i < msg->size(); i++){

			int Result = Encode_Character((char) msg->at(i), result, Root);
			
				// If there is an error in converting it return.
			if (Result != 0){
				Root->DeleteChildren();
				delete Root;

				return Result;
			}
		}

		return 0;
	}








	private: int Decode_Character(std::string &Input, char &Result, Node* &Root){

		// We use the Input to go left or right on the tree unless it's the zero node then we look through the characters.

		Node* Current_Search = Root;
		
		while (Current_Search != NULL){

				// Catch if the input is empty.
			if (Input.size() < 1){
				return 1;
			}

				// Check if it is the zero node, if so search though it.
			if (Current_Search->Weight == 0){

					// We make sure there is a character
				if (Input.size() < 2 || Current_Search->Character_Set.size() < 1){
					return 2;
				}

					// Go though the characters until we hit a zero.
				for (unsigned int i = 0; i <= Current_Search->Character_Set.size(); i++){
					if (Input.size() < 1){
						return 4;
					}

					char Local_Char = Input[0];
					Input.erase(0, 1);
						
						// Check if we accept or continue.
					if (Local_Char == '0'){

							// Accept that character
						Result = Current_Search->Character_Set[i - 1];
												
							// Remove the character from the zero node.
						if (Current_Search->Character_Set.size() == 1){
							Current_Search->Character_Set = "";
						}else{
								// Incase the letter is the last one.
							if ((Current_Search->Character_Set.size() - 1) == (i - 1)){
									// Delete the last character.
								Current_Search->Character_Set.erase(Current_Search->Character_Set.size() - 1, Current_Search->Character_Set.size() - 1);
							}else{
									// Replace the character with the last one and delete the last one.
								Current_Search->Character_Set[i - 1] = Current_Search->Character_Set[Current_Search->Character_Set.size() - 1];
								Current_Search->Character_Set.erase(Current_Search->Character_Set.size() - 1, Current_Search->Character_Set.size() - 1);
							}
						}

							// Because we pulled out a new character we push it onto the huffman tree.
						Insert_Character_Into_Huffman(Current_Search, Result);
						
						if (Current_Search->Parent != NULL){
							Current_Search = Current_Search->Parent;
							
							Adjust_Character_Weight_Huffman(Current_Search);
						}

							// Reset the root back at root.
						while (Root->Parent != NULL){
							Root = Root->Parent;
						}

						return 0;
					}else{
							// Move down one.
						Result = Current_Search->Character_Set[i];
					}
				}

				// We for some reason did not exit with return 0 inside that loop above so error
				return 5;
			}

			// check if it is a character, if so accept that character.
			if (Current_Search->Letter != -1){
				// The node contains a letter so we accept the letter and return.
				Result = Current_Search->Letter;
				Adjust_Character_Weight_Huffman(Current_Search);

				return 0;
			}

			// Follow the direction.
			char Working_Char = Input[0];
			Input.erase(0, 1);

			if (Working_Char == '1'){
				//Go to the right
				Current_Search = Current_Search->Right_Child;
			}else{
				Current_Search = Current_Search->Left_Child;
			}
		}


		return 0;
	}

	public:	int decode(std::string *msg, std::string &result, int rbuff_size){

		//The root of the huffman tree.
		Node* Root = NULL;

		//Create a zero node and put it into the tree.
		Node Zero_Node;
		Zero_Node.Character_Set = *Alphabet;

		Root = &Zero_Node;

		std::string Local_Msg = *msg;

		// Loop through all of the characters.
		while (Local_Msg.size() > 0){

			char Local_Results = -1;

			int Result = Decode_Character(Local_Msg, Local_Results, Root);

			result += Local_Results;

			// If there is an error in converting it return.
			if (Result != 0){
				Root->DeleteChildren();
				delete Root;

				return Result;
			}
		}

		return 0;
	}

		// Default Constructor.
	public:	AdaptiveHuffman(string *alphabet){
		Alphabet = new string(*alphabet);
	}

		// Default Destructor.
	public:	~AdaptiveHuffman(){

			// Such clean up.
		delete Alphabet;	
	}
    
};

#endif
