//-----------------------------------------------------------------------------
// TmxMap.cpp
//
// Copyright (c) 2010-2012, Tamir Atias
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL TAMIR ATIAS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Tamir Atias
//-----------------------------------------------------------------------------
#include <ticpp/tinyxml.h>
#include <stdio.h>

#include "TmxMap.h"
#include "TmxTileset.h"
#include "TmxLayer.h"
#include "TmxObjectGroup.h"
#include <Ref.h>
#include <FileStream.h>

using std::vector;
using std::string;

namespace Tmx 
{
	Map::Map() 
		: file_name()
		, file_path()
		, version(0.0)
		, orientation(TMX_MO_ORTHOGONAL)
		, width(0)
		, height(0)
		, tile_width(0)
		, tile_height(0)
		, layers()
		, tilesets() 
		, has_error(false)
		, error_code(0)
		, error_text()
	{}

	Map::~Map() 
	{
		// Iterate through all of the layers and delete each of them.
		vector< Layer* >::iterator lIter;
		for (lIter = layers.begin(); lIter != layers.end(); ++lIter) 
		{
			Layer *layer = (*lIter);

			if (layer) 
			{
				delete layer;
				layer = NULL;
			}
		}

		// Iterate through all of the tilesets and delete each of them.
		vector< Tileset* >::iterator tsIter;
		for (tsIter = tilesets.begin(); tsIter != tilesets.end(); ++tsIter) 
		{
			Tileset *tileset = (*tsIter);
			
			if (tileset) 
			{
				delete tileset;
				tileset = NULL;
			}
		}
	}

	void Map::ParseFile(const string &fileName) 
	{
		file_name = fileName;

		int lastSlash = fileName.find_last_of("/");

		// Get the directory of the file using substring.
		if (lastSlash > 0) 
		{
			file_path = fileName.substr(0, lastSlash + 1);
		} 
		else 
		{
			file_path = "";
		}

		has_error = true;
		error_code = TMX_COULDNT_OPEN;
		error_text = "Could not open the file.";
		yam2d::Ref<yam2d::Stream> s = new yam2d::FileStream(fileName.c_str(), yam2d::FileStream::READ_ONLY );
	
		// Check if the file size is valid.
		if (s->available() <= 0)
		{
			has_error = true;
			error_code = TMX_INVALID_FILE_SIZE;
			error_text = "The size of the file is invalid.";
			return;
		}
		
		has_error = false;
		error_code = 0;
		error_text = "";

		// Allocate memory for the file and read it into the memory.
		std::vector<char> fileText;
		fileText.resize( s->available() );
		s->read( &fileText[0], fileText.size() );

		// Copy the contents into a C++ string and delete it from memory.
		std::string text(&fileText[0], &fileText[0]+fileText.size());
		
		ParseText(text);


	}

	void Map::ParseText(const string &text) 
	{
		// Create a tiny xml document and use it to parse the text.
		TiXmlDocument doc;
		doc.Parse(text.c_str());
	
		// Check for parsing errors.
		if (doc.Error()) 
		{
			has_error = true;
			error_code = TMX_PARSING_ERROR;
			error_text = doc.ErrorDesc();
			return;
		}

		TiXmlNode *mapNode = doc.FirstChild("map");
		TiXmlElement* mapElem = mapNode->ToElement();

		// Read the map attributes.
		mapElem->Attribute("version", &version);
		mapElem->Attribute("width", &width);
		mapElem->Attribute("height", &height);
		mapElem->Attribute("tilewidth", &tile_width);
		mapElem->Attribute("tileheight", &tile_height);

		// Read the orientation
		std::string orientationStr = mapElem->Attribute("orientation");

		if (!orientationStr.compare("orthogonal")) 
		{
			orientation = TMX_MO_ORTHOGONAL;
		} 
		else if (!orientationStr.compare("isometric")) 
		{
			orientation = TMX_MO_ISOMETRIC;
		}

		// Read the map properties.
		const TiXmlNode *propertiesNode = mapElem->FirstChild("properties");
		if (propertiesNode) 
		{
			properties.Parse(propertiesNode);
		}

		// Iterate through all of the tileset elements.
		const TiXmlNode *tilesetNode = mapNode->FirstChild("tileset");
		while (tilesetNode) 
		{
			// Allocate a new tileset and parse it.
			Tileset *tileset = new Tileset();
			tileset->Parse(tilesetNode->ToElement());

			// Add the tileset to the list.
			tilesets.push_back(tileset);

			tilesetNode = mapNode->IterateChildren("tileset", tilesetNode);
		}

		// Find all layers and object groups.
		TiXmlNode *layerNode = mapNode->FirstChild();
		while( layerNode != 0 )
		{
			if( std::string("layer") == layerNode->Value() )
			{
				// Allocate a new layer and parse it.
				TileLayer *layer = new TileLayer(this);
				layer->Parse(layerNode);

				// Add the layer to the list.
				layers.push_back(layer);
			}

			if( std::string("objectgroup") == layerNode->Value() )
			{
				// Allocate a new object group and parse it.
				ObjectLayer *objectGroup = new ObjectLayer(this);
				objectGroup->Parse(layerNode);
		
				// Add the object group to the list.
				layers.push_back(objectGroup);
			}

			layerNode = layerNode->NextSibling();
		}
	}

	int Map::FindTilesetIndex(int gid) const
	{
		// Clean up the flags from the gid (thanks marwes91).
		gid &= ~(FlippedHorizontallyFlag | FlippedVerticallyFlag | FlippedDiagonallyFlag);

		for (int i = tilesets.size() - 1; i > -1; --i) 
		{
			// If the gid beyond the tileset gid return its index.
			if (gid >= tilesets[i]->GetFirstGid()) 
			{
				return i;
			}
		}
		
		return -1;
	}

	const Tileset *Map::FindTileset(int gid) const 
	{
		for (int i = tilesets.size() - 1; i > -1; --i) 
		{
			// If the gid beyond the tileset gid return it.
			if (gid >= tilesets[i]->GetFirstGid()) 
			{
				return tilesets[i];
			}
		}
		
		return NULL;
	}
};
