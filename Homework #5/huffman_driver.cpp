/*
	Written by David Harkins.

	Extra credit feature was implemented, -B

*/
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include "adaptivehuffman.h"

using namespace std;


	// Load file by full path and return a pointer to a char *str or null.
char *LoadFileByName(std::string argument){

	std::streampos size;

	char * memblock;

	std::ifstream file(argument.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open()){

		size = file.tellg();
		unsigned int IntSize = (unsigned int) size;
		IntSize++;
		memblock = new char[IntSize];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();

			//I don't know why but I have to patch it to actually have a end.
		memblock[(unsigned int) size] = '\0';

	}else{
		return NULL;
	}

	return memblock;
}
	
	// Converts from char * to string *
string *LoadFile_by_name(string *argument){

	char *Temp = LoadFileByName(*argument);

		// Catch if there is no file.
	if (Temp == NULL){
		return NULL;
	}
	
		// The file was loaded so return it.
	string *Data = new string(Temp);

		// Clean up.
	delete Temp;
	
	return Data;
}

std::string Convert_Binary_To_String(unsigned long argument){
	return std::bitset< 8 >(argument).to_string();
}

unsigned long Convert_String_To_Binary(std::string argument){
	return (unsigned char) std::bitset< 8 >(argument).to_ulong();
}

	// Loads the "Encode.binary"
std::string Load_Binary_Dump(){

	std::string result = "";
	
	streampos size;
	char * memblock;

	ifstream file("Encode.binary", ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[(unsigned int) size];
		file.seekg(0, ios::beg);
		file.read(memblock, size);
		file.close();

		for (unsigned i = 0; i < size; i++){
			result += Convert_Binary_To_String((unsigned char) memblock[i]);
		}

		delete[] memblock;
	}
	
		// We know the encoding has to start with a 1 so remove the trash from the start of the encoding.
	bool Trigger = false;

	std::string Cleaned_Result;

	for (unsigned int i = 0; i < result.size(); i++){
		if (Trigger || result[i] == '1'){
			Trigger = true;
			Cleaned_Result += result[i];
		} 
	}
	
	return Cleaned_Result;
}

	// Saves to "Encode.binary"
void Dump_To_Binary(std::string argument){
		
	FILE* pFile;
	pFile = fopen("Encode.binary", "wb");
	
		// Write out the only uneven one.
	if (pFile){

		std::string chunk = "";

		// fill in 8 binary bits. (argument.size() % 8)
		for (unsigned int i = 0; i < (argument.size() % 8); i++){
			chunk += argument[i];
		}

		unsigned char ss = (unsigned char) Convert_String_To_Binary(chunk);
		unsigned char *ss1 = &ss;

		fwrite(ss1, 1, sizeof(unsigned char), pFile);
	}

		for (unsigned int j = (argument.size() % 8); j < argument.size(); j++){
			
			std::string chunk = "";
			
			// fill in 8 binary bits. 
			for (unsigned int i =  j; i < argument.size() && (i - j) < sizeof(unsigned char) * 8; i++){
				chunk += argument[i];
			}

			j += 7;

			unsigned char ss = (unsigned char) Convert_String_To_Binary(chunk);
			unsigned char *ss1 = &ss;

			std::string test = Convert_Binary_To_String(ss);

			fwrite(ss1, 1, sizeof(unsigned char), pFile);
		}

	fclose(pFile);
}


/*

	Commandline useage guide:

	Useage: huffman_driver <alphabet file> <messagefile> <encode or decode> <-B for if the file should be dumped as a binary file or not>

	Example: huffman_driver "alphabet.txt" "message.txt" encode -B

*/
int main(int argc, char const *argv[]){

		// We normally have 4 arguments, the 5th is if the user wants it dumped in binary.
    if(argc < 4 || argc > 5){
			// Invalid number of files
        cout << "Invald number of command line arguments" << endl;
        return 1;
    }

		// Convert the arguments to more useable
    string *alphabet_file_name = new string(argv[1]);
    string *message_file_name = new string(argv[2]);
    string *operation = new string(argv[3]); // "encode" or "decode"

	bool Create_Binary = false;
		
		// If there is a fifth argument check if it's correct else do nothing.
	if (argc == 5){
		string *Argument_5 = new string(argv[4]);

			//Compare.
		if ("-B" == *Argument_5){
			Create_Binary = true;
		}

		delete Argument_5;
	}

		// Load the message from the file.
	string *Loaded_Message = LoadFile_by_name(message_file_name);

		// Load the alphabet from the file.
	string *Loaded_Alphabet = LoadFile_by_name(alphabet_file_name);


		// Catch if the message file does not exist.
	if (Loaded_Message == NULL || Loaded_Alphabet == NULL){
			// Invalid file path.
		cout << "Invalid file path, unable to load." << endl;
		return 1;
	}

		// Create a instance of the huffman class with the argument alphabet.
	AdaptiveHuffman Huffman(Loaded_Alphabet);

		// Encode the message.
	if (*operation == "encode"){

			// Attach a single character the same as the last into the message. (just trust me)
		string Loaded_Message_local = *Loaded_Message;
		Loaded_Message_local += Loaded_Message_local[Loaded_Message_local.size() - 1];

		std::string result = "";
		
		int Result_Code = Huffman.encode(&Loaded_Message_local, result, Loaded_Message->size());

			// If there was an error quit.
		if (Result_Code != 0){
			return Result_Code;
		}

			// Print the encoded message.
		if (result.size() > 0){
			std::cout << result; // << std::endl; // No newline because it's going into a file.
		}

			// If we should also dump to a binary file
		if (Create_Binary){
			Dump_To_Binary(result);
		}
	}

		// Decode the message.
	if (*operation == "decode"){
		
			// If we should load the binary file instead of the message 
		if (Create_Binary){
			delete Loaded_Message;
			
			Loaded_Message = new string(Load_Binary_Dump());
		}

		std::string result = "";

		int Result_Code = Huffman.decode(Loaded_Message, result, Loaded_Message->size());

		//result.pop_back();
		result.erase(result.size() - 1, result.size() - 1);

		// If there was an error quit.
		if (Result_Code != 0){	
				// Normally I would return an error but we just need to clip the last character off. //return Result_Code;
		}

		// Print the encoded message.
		if (result.size() > 0){
			std::cout << result; // << std::endl; // no newline because it's going into a file.
		}
	}


		// Do the same thing as my goal in the waste, kill everything.
	delete Loaded_Alphabet;
	delete Loaded_Message;

	delete alphabet_file_name;
	delete message_file_name;
	delete operation;


    return 0;
}
