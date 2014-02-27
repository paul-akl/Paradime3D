#ifndef LOADER_CONFIG
#define LOADER_CONFIG

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ErrorHandler.h"
#include "Math.h"

class ConfigFile
{
public:
	
	enum typeID
	{
		emptyType,
		boolType,
		intType,
		floatType,
		doubleType,
		stringType,
		vec2fType,
		vec3fType
	};
	union dataTypes
	{
		bool		boolValue;
		int			intValue;
		float		floatValue;
		double		doubleValue;
		std::string *stringValue;
		Math3d::Vec2f *vec2f;
		Math3d::Vec3f *vec3f;
	};

	class BaseValue
	{
	public:
		virtual void setParameter(std::string value_arg) = 0;
		virtual std::string getString() = 0;

		virtual bool getBool() = 0;
		virtual int getInt() = 0;
		virtual float getFloat() = 0;
		virtual Math3d::Vec2f getVec2f() = 0;
		virtual Math3d::Vec3f getVec3f() = 0;

		int type;

	protected:
		dataTypes value;
	};
	class BoolValue : public BaseValue
	{
	public:
		BoolValue(std::string value_arg)
		{
			type = boolType;
			setParameter(value_arg);
		}
		std::string getString()
		{
			return Message::toString(value.boolValue);
		}
		void setParameter(std::string value_arg)
		{
			if(value_arg == "1" || value_arg == "true" || value_arg == "TRUE")
			{
				value.boolValue = true;
			}
			else
			{
				value.boolValue = false;
			}
		}
		
		bool getBool()
		{
			return value.boolValue;
		}
		int getInt()
		{
			return value.boolValue ? 0 : 1;
		}
		float getFloat()
		{
			return value.boolValue ? 0.0f : 1.0f;
		}
		Math3d::Vec2f getVec2f()
		{
			return value.boolValue ? Math3d::Vec2f(0.0) : Math3d::Vec2f(1.0);
		}
		Math3d::Vec3f getVec3f()
		{
			return value.boolValue ? Math3d::Vec3f(0.0) : Math3d::Vec3f(1.0);
		}
	};
	class IntValue : public BaseValue
	{
	public:
		IntValue(std::string value_arg)
		{
			type = intType;
			setParameter(value_arg);
		}
		std::string getString()
		{
			return Message::toString(value.intValue);
		}
		void setParameter(std::string value_arg)
		{
			value.intValue = std::atoi(value_arg.c_str());
		}
		
		bool getBool()
		{
			return value.intValue == 0 ? false : true;
		}
		int getInt()
		{
			return value.intValue;
		}
		float getFloat()
		{
			return (float)value.intValue;
		}
		Math3d::Vec2f getVec2f()
		{
			return Math3d::Vec2f((float)value.intValue);
		}
		Math3d::Vec3f getVec3f()
		{
			return Math3d::Vec3f((float)value.intValue);
		}
	};
	class FloatValue : public BaseValue
	{
	public:
		FloatValue(std::string value_arg)
		{
			type = floatType;
			setParameter(value_arg);
		}
		std::string getString()
		{
			return Message::toString(value.floatValue);
		}
		void setParameter(std::string value_arg)
		{
			value.floatValue = (float)std::atof(value_arg.c_str());
		}
		
		bool getBool()
		{
			return value.floatValue == 0.0 ? false : true;
		}
		int getInt()
		{
			return (int)value.floatValue;
		}
		float getFloat()
		{
			return value.floatValue;
		}
		Math3d::Vec2f getVec2f()
		{
			return Math3d::Vec2f(value.floatValue);
		}
		Math3d::Vec3f getVec3f()
		{
			return Math3d::Vec3f(value.floatValue);
		}
	};
	class StringValue : public BaseValue
	{
	public:
		StringValue(std::string value_arg)
		{
			type = stringType;
			setParameter(value_arg);
		}
		std::string getString()
		{
			return *value.stringValue;
		}
		void setParameter(std::string value_arg)
		{
			value.stringValue = new std::string(value_arg);
		}

		bool getBool()
		{
			return std::atoi((value.stringValue)->c_str()) ==  0 ? true : false;
		}
		int getInt()
		{
			return std::atoi((value.stringValue)->c_str());
		}
		float getFloat()
		{
			return (float)std::atof((value.stringValue)->c_str());
		}
		Math3d::Vec2f getVec2f()
		{
			return Math3d::Vec2f((float)std::atof((value.stringValue)->c_str()));
		}
		Math3d::Vec3f getVec3f()
		{
			return Math3d::Vec3f((float)std::atof((value.stringValue)->c_str()));
		}
	};
	class Vec2fValue : public BaseValue
	{
	public:
		Vec2fValue(std::string value_arg)
		{
			type = vec2fType;
			setParameter(value_arg);
		}
		std::string getString()
		{
			return Message::toString(value.vec2f->x) + ", " + Message::toString(value.vec2f->y);
		}
		void setParameter(std::string value_arg)
		{
			std::string xString, yString;
			std::vector<std::string>::size_type index;
			for(index = 0; value_arg[index] != ','; index++)
				xString += value_arg[index];	index++;
			for(; index < value_arg.size(); index++)
				yString += value_arg[index];
			
			value.vec2f = new Math3d::Vec2f((float)std::atof(xString.c_str()),
											(float)std::atof(yString.c_str()));
		}
		
		bool getBool()
		{
			return value.vec2f->x == 0.0 ? false : true;
		}
		int getInt()
		{
			return (int)value.vec2f->x;
		}
		float getFloat()
		{
			return (float)value.vec2f->x;
		}
		Math3d::Vec2f getVec2f()
		{
			return *(value.vec2f);
		}
		Math3d::Vec3f getVec3f()
		{
			return Math3d::Vec3f(value.vec2f->x, value.vec2f->y, 0.0);
		}
	};
	class Vec3fValue : public BaseValue
	{
	public:
		Vec3fValue(std::string value_arg)
		{
			type = vec3fType;
			setParameter(value_arg);
		}
		std::string getString()
		{
			return Message::toString(value.vec3f->x) + ", " + Message::toString(value.vec3f->y) + ", " + Message::toString(value.vec3f->z);
		}
		void setParameter(std::string value_arg)
		{
			std::string xString, yString, zString;
			std::vector<std::string>::size_type index;
			for(index = 0; value_arg[index] != ','; index++)
				xString += value_arg[index];	index++;
			for(; value_arg[index] != ','; index++)
				yString += value_arg[index];	index++;
			for(; index < value_arg.size(); index++)
				zString += value_arg[index];

			value.vec3f = new Math3d::Vec3f((float)std::atof(xString.c_str()),
											(float)std::atof(yString.c_str()),
											(float)std::atof(zString.c_str()));
		}
		
		bool getBool()
		{
			return value.vec3f->x == 0.0 ? false : true;
		}
		int getInt()
		{
			return (int)value.vec3f->x;
		}
		float getFloat()
		{
			return (float)value.vec3f->x;
		}
		Math3d::Vec2f getVec2f()
		{
			return Math3d::Vec2f(value.vec3f->x, value.vec3f->y);
		}
		Math3d::Vec3f getVec3f()
		{
			return *(value.vec3f);
		}
	};
	
	class ArrayNode;
	class Node
	{
	public:
		Node(std::string name_arg)
		{ 
			name = name_arg;
			value = NULL;
		}
		~Node()
		{
			if(value != NULL)
				delete value;
			childNodes.clear();
			arrayNodes.clear();
		}

		Node *getNode(std::string name_arg)
		{
			for(std::vector<ConfigFile::Node*>::size_type i=0; i < childNodes.size(); i++)
				if(childNodes[i]->name == name_arg)
					return childNodes[i];
			return NULL;
		}
		Node *getValueNode(std::string name_arg)
		{
			
			Node *node_red = getNode(name_arg);
			return node_red;
			/*
			Node *node_red = getNode(name_arg);
			if(node_red != NULL)
			{
				if(node_red->childNodes[0] != NULL)
					return node_red->childNodes[0];
			}
			else
				return NULL;*/
		}

		BaseValue *value;
		std::vector<Node*> childNodes;
		std::vector<ArrayNode*> arrayNodes;
		std::string name;
	};
	class ArrayNode
	{
	public:
		ArrayNode()
		{

		}
		~ArrayNode()
		{
			nodes.clear();
			arrayNodes.clear();
		}
		
		Node *getNode(std::string name_arg)
		{
			for(std::vector<ConfigFile::Node*>::size_type i=0; i < nodes.size(); i++)
				if(nodes[i]->name == name_arg)
					return nodes[i];
			return NULL;
		}
		Node *getValueNode(std::string name_arg)
		{
			Node *node_red = getNode(name_arg);
			return node_red;
			//if(node_red != NULL)
			//{
			//	if(node_red->childNodes[0] != NULL)
			//		return node_red->childNodes[0];
			//}
			//else
			//	return NULL;
		}

		std::vector<Node*> nodes;
		std::vector<ArrayNode*> arrayNodes;
	};

	ConfigFile();
	~ConfigFile();

	void import();
	void import(std::string fileName_arg);
	std::string getString();
	Node *getRootNode();

	std::string configFileName;

private:
	std::string configFileString;
	Node *rootNode;

	void loadFromFile();
	void generateEntries();
	std::string getName(std::vector<std::string>::size_type &index_arg);
	BaseValue *getBaseValue(std::string value_arg);
	Node *fillNode(std::vector<std::string>::size_type &index_arg);
	ArrayNode *fillArrayNode(std::vector<std::string>::size_type &index_arg);

	std::string getString(Node &node_arg);
	std::string getString(ArrayNode &arrayNode_arg);
};

#endif