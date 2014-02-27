
#include <ctype.h>

#include "Loader_Config.h"

ConfigFile::ConfigFile()
{
	rootNode = NULL;
}
ConfigFile::~ConfigFile()
{
	if(rootNode != NULL)
		delete rootNode;
}

void ConfigFile::import()
{
	import(configFileName);
}
void ConfigFile::import(std::string fileName_arg)
{
	configFileName = fileName_arg;
	rootNode = NULL;

	try
	{
		loadFromFile();
		generateEntries();
	}
	catch(Message::messageCode error)
	{
		error.display();
	}
}
std::string ConfigFile::getString()
{
	if(rootNode != NULL)
	{
		std::string string_ret = "Config file \"" + rootNode->name + "\" :\n------------------------------------------------";
		for(std::vector<ConfigFile::Node*>::size_type i=0; i < rootNode->childNodes.size(); i++)
			string_ret += getString(*rootNode->childNodes[i]);
		string_ret += "\n------------------------------------------------";
		return string_ret;
	}
	return "No data exist.";
}
ConfigFile::Node* ConfigFile::getRootNode()
{
	return rootNode;
}

void ConfigFile::loadFromFile()
{
	std::ifstream configFile;
	std::string singleLine, tempConfigString;

	configFile.open(configFileName, std::ios::in);
	
	if(configFile.fail())
	{
		configFile.close();
		throw Message::messageCode(MSG_FATAL_ERROR, MSG_OBJECT, configFileName + ": Has failed to load.");
	}
	
	while(!configFile.eof())
	{
		// Get every line, to be processed
		std::getline(configFile, singleLine);

		// If it contains commentary marks, discard the whole line
		if(singleLine.size() >= 2 && singleLine[0] == '/' && singleLine[1] == '/')
			continue;

		// Add each line to the whole file string
		tempConfigString += singleLine;
	}

	// Remove all spaces and horizontal tabs, except from in between quotation marks
	for(std::vector<std::string>::size_type i=0; i < tempConfigString.size(); i++)
	{
		if(tempConfigString[i] == '"')						// If it's a quatation mark
		{
			configFileString += tempConfigString[i];		// put the first quotation mark in the final string
			for(i++; tempConfigString[i] != '"'; i++)		// itterate to find the second quatation mark
				configFileString += tempConfigString[i];	// put every char (that is in between quotation marks) in the final string
		}

		if(tempConfigString[i] != ' ' && tempConfigString[i] != '\t')	// If the char is not space or tab
			configFileString += tempConfigString[i];					// Copy it to the final string
	}
	//std::cout << configFileString << std::endl;
	configFile.close();
}
void ConfigFile::generateEntries()
{
	std::vector<std::string>::size_type index = 0;
	rootNode = new Node("root");
	//Node *currentNode = rootNode;

	std::string nameTemp;

	for(; configFileString[index] != '{'; index++);
		
	do
	{
		rootNode->childNodes.push_back(fillNode(index));

		//std::cout << "root node: " << (rootNode->childNodes[rootNode->childNodes.size() - 1])->name << std::endl;
	}while(configFileString[index] == ',');
	//std::cout << configFileString[index] << std::endl;
}
std::string ConfigFile::getName(std::vector<std::string>::size_type &index_arg)
{
	std::string name_ret;
	// Search for the first quotation marks
	for(; index_arg < configFileString.size() && configFileString[index_arg] != '"'; index_arg++);

	// Check if it's end of the string, to not couse out of bounds error
	if(index_arg < configFileString.size())
		index_arg++;	// Increment, so search for second quotation marks won't find first quotation marks

	// Search for the second quotation marks
	for(; index_arg < configFileString.size() && configFileString[index_arg] != '"'; index_arg++)
		name_ret += configFileString[index_arg];
	
	// Check if it's end of the string, to not couse out of bounds error
	if(index_arg < configFileString.size())
		index_arg++;	// Increment, so index is not on second quotation marks
	else
		index_arg--;	// Decrement, so end character ( "}" ) can be found, by main while loop
	return name_ret;
}
ConfigFile::BaseValue *ConfigFile::getBaseValue(std::string value_arg)
{
	if(value_arg == "true" || value_arg == "false")	// If value is bool
		return new BoolValue(value_arg);

	int decimalPoints = 0, commas = 0;
	for(std::vector<std::string>::size_type i=0; i < value_arg.size(); i++)
	{
		if(isalpha(value_arg[i]) && (i < value_arg.size() || value_arg[i] != 'f'))	// If value is string
			return new StringValue(value_arg);

		if(value_arg[i] == '.')
			decimalPoints++;
		if(value_arg[i] == ',')
			commas++;
	}

	if(decimalPoints == 0)	// If value is integer
	{
		return new IntValue(value_arg);
	}
	else
	{
		if(decimalPoints == 3 && commas == 2)	// If value is vector of 3 floats
		{
			return new Vec3fValue(value_arg);
		}
		if(decimalPoints == 2 && commas == 1)	// If value is vector of 2 floats
		{
			return new Vec2fValue(value_arg);
		}

		if(decimalPoints == 1 && commas == 0)
		{
			if(value_arg[value_arg.size() - 1] == 'f')	// If value is float
				return new FloatValue(value_arg);
			else										// If value is double (not implemented yet)
				return new FloatValue(value_arg);
		}
		else
			return new BoolValue("false");	// If value is incorrect or cannot be read, just create a bool and set it to false
	}
}
ConfigFile::Node *ConfigFile::fillNode(std::vector<std::string>::size_type &index_arg)
{
	Node *node_ret = new Node(getName(index_arg));

	if(configFileString[index_arg] == ':')
	{
		index_arg++;
		switch(configFileString[index_arg])
		{
			case '"':
				node_ret->value = getBaseValue(getName(index_arg));
				//std::cout << node_ret->value->getString() << std::endl;
			break;
			
			case '{':
				do
				{
					index_arg++;
					node_ret->childNodes.push_back(fillNode(index_arg));

				} while(configFileString[index_arg] == ',');
				index_arg++;
			break;

			case '[':
				do
				{
					index_arg++;
					node_ret->arrayNodes.push_back(fillArrayNode(index_arg));
				} while(configFileString[index_arg] == ',');
				if(configFileString[index_arg] != ']')
					Message::show(MSG_WARNING, MSG_FILELOADER, "Missing \" ] \" sign.");
				else
					index_arg++;
			break;
			
			Message::show(MSG_WARNING, MSG_FILELOADER, "Unknown sing " + Message::toString(configFileString[index_arg]) + ". Should be \" \" \", \" { \" or \" [ \".");
		}
	}
	else
	{
		Message::show(MSG_WARNING, MSG_FILELOADER, "Missing \" : \" sign.");
	}
	return node_ret;
}
ConfigFile::ArrayNode *ConfigFile::fillArrayNode(std::vector<std::string>::size_type &index_arg)
{
	ArrayNode *arrayNode_ret = new ArrayNode();
	do
	{
		arrayNode_ret->nodes.push_back(fillNode(index_arg));
		//std::cout << arrayNode_ret->nodes[arrayNode_ret->nodes.size() - 1]->name << " | Array Node" << std::endl;
	} while(configFileString[index_arg] == ',');

	index_arg++;
	return arrayNode_ret;
}

std::string ConfigFile::getString(Node &node_arg)
{
	std::string string_ret = "\n" + node_arg.name;
	if(node_arg.value != NULL)
		string_ret += " : " + node_arg.value->getString();
	else
	{
		string_ret += " -";
		for(std::vector<ConfigFile::Node*>::size_type i=0; i < node_arg.childNodes.size(); i++)
			string_ret += getString(*node_arg.childNodes[i]);
		for(std::vector<ConfigFile::ArrayNode*>::size_type i=0; i < node_arg.arrayNodes.size(); i++)
			string_ret += getString(*node_arg.arrayNodes[i]);
	}
	return string_ret;
}
std::string ConfigFile::getString(ArrayNode &arrayNode_arg)
{
	std::string string_ret;
	for(std::vector<ConfigFile::Node*>::size_type i=0; i < arrayNode_arg.nodes.size(); i++)
		string_ret += getString(*arrayNode_arg.nodes[i]);
	for(std::vector<ConfigFile::ArrayNode*>::size_type i=0; i < arrayNode_arg.arrayNodes.size(); i++)
		string_ret += getString(*arrayNode_arg.arrayNodes[i]);
	return string_ret;
}