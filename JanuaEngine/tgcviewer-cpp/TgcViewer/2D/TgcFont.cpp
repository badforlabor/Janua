/////////////////////////////////////////////////////////////////////////////////
// TgcViewer-cpp
// 
// Author: Matias Leone
// 
/////////////////////////////////////////////////////////////////////////////////



#include "TgcViewer/2D/TgcFont.h"
#include "TgcViewer/GuiController.h" //required by forward declaration
using namespace TgcViewer;


const string TgcFont::MICROSOFT_SANS_SERIF_12 = "Microsoft_Sans_Serif_12";
const string TgcFont::MICROSOFT_SANS_SERIF_16 = "Microsoft_Sans_Serif_16";
const string TgcFont::MICROSOFT_SANS_SERIF_20 = "Microsoft_Sans_Serif_20";
const string TgcFont::MICROSOFT_SANS_SERIF_26 = "Microsoft_Sans_Serif_26";


TgcFont::TgcFont()
{
}


TgcFont::TgcFont(const TgcFont& other)
{
}


TgcFont::~TgcFont()
{
}


void TgcFont::loadFromFile(string id, string bitmapPath, string dataPath)
{
	this->id = id;
	this->texture = TgcTexture::createTexture(bitmapPath);

	//Parse csv output file generated by http://www.codehead.co.uk/cbfg/
	//We are only interested in "Cell Width", "Cell Height", "Font Name", "Font Height" and "Char N Base Width"
	const string S_SEP = ",";
	const string S_CELL_WIDTH = "Cell Width";
	const string S_CELL_HEIGHT = "Cell Height";
	const string S_FONT_NAME = "Font Name";
	const string S_FONT_HEIGHT = "Font Height";
	const string S_CHAR = "Char ";
	const string S_BASE_WIDTH = " Base Width";

	ifstream input(dataPath);
	if(!input.is_open())
	{
		GuiController::Instance->logger->logError("Error loading font: " + id + ". Could not open CSV font data: " + dataPath);
		return;
	}
	for( string line; getline( input, line ); )
	{
		//Cell Width,32
		if(line.find(S_CELL_WIDTH) != string::npos)
		{
			this->cellSize.X = TgcParserUtils::parseFloat(TgcParserUtils::trim(line.substr(line.find(S_SEP) + 1)));
		}
		//Cell Height,32
		else if(line.find(S_CELL_HEIGHT) != string::npos)
		{
			this->cellSize.Y = TgcParserUtils::parseFloat(TgcParserUtils::trim(line.substr(line.find(S_SEP) + 1)));
		}
		//Font Name,Microsoft Sans Serif
		else if(line.find(S_FONT_NAME) != string::npos)
		{
			this->name = TgcParserUtils::trim(line.substr(line.find(S_SEP) + 1));
		}
		//Font Height,16
		else if(line.find(S_FONT_HEIGHT) != string::npos)
		{
			this->height = TgcParserUtils::parseInt(TgcParserUtils::trim(line.substr(line.find(S_SEP) + 1)));
		}
		//Char 32 Base Width,3
		else if(line.find(S_CHAR) != string::npos && line.find(S_BASE_WIDTH) != string::npos)
		{
			string aux = line.substr(0, line.find(S_BASE_WIDTH));
			aux = aux.substr(4);
			int c = TgcParserUtils::parseInt(TgcParserUtils::trim(aux));
			int width = TgcParserUtils::parseInt(TgcParserUtils::trim(line.substr(line.find(S_SEP) + 1)));
			this->charWidth[c] = width;
		}
	}
	input.close();
}

void TgcFont::getUvExtremes(const char c, Vector2& uvMin, Vector2& uvMax)
{
	const int n = (int)c;

	//Compute min and max UV
	uvMin.U = (float)(n % (int)(this->texture->width / this->cellSize.X)) * this->cellSize.X;
	uvMin.V = (float)(n / (int)(this->texture->width / this->cellSize.X)) * this->cellSize.Y;
	uvMax = uvMin + Vector2((float)this->charWidth[n], (float)this->height);

	//Normalize values
	uvMin.X /= this->texture->width;
	uvMax.X /= this->texture->width;
	uvMin.Y /= this->texture->height;
	uvMax.Y /= this->texture->height;
}

void TgcFont::dispose()
{
	texture->dispose();
}

