
#include "BlockDictionary.hpp"
#include "BlockTemplate.hpp"

int main(int argc, char * argv[])
{
	printf("Starting block dictionary test\n");
	BlockDictionary * dict = new BlockDictionary();

	printf("\tAdding attributes (i, f, b)\n");
	dict->AddAttribute("iID");
	dict->AddAttribute("bOcclude");
	dict->AddAttribute("fTexture");
	dict->AddAttribute("sName");

	printf("\tCreating template\n");
	BlockTemplate * temp1 = new BlockTemplate();
	temp1->AddAttribute("iID", 1);
	temp1->AddAttribute("bOcclude", false);
	temp1->AddAttribute("fTexture", 0.0f);
	temp1->AddAttribute("sName", "Noodles");

	printf("\tCreating template\n");
	BlockTemplate * temp2 = new BlockTemplate();
	temp2->AddAttribute("iID", 2);
	temp2->AddAttribute("bOcclude", false);
	temp2->AddAttribute("fTexture", 0.0f);
	temp2->AddAttribute("sName", "Stuff");

	BlockTemplate * temp3 = new BlockTemplate();
	temp3->AddAttribute("iID", 13);
	temp3->AddAttribute("bOcclude", true);
	temp3->AddAttribute("fTexture", 0.0f);
	temp3->AddAttribute("sName", "Stone");

	BlockTemplate * temp4 = new BlockTemplate();
	temp4->AddAttribute("iID", 14);
	temp4->AddAttribute("bOcclude", true);
	temp4->AddAttribute("fTexture", 1.0f);
	temp4->AddAttribute("sName", "Wood");

	printf("\tAdding templates\n");
	dict->AddTemplate(temp1);
	dict->AddTemplate(temp2);
	dict->AddTemplate(temp3);
	dict->AddTemplate(temp4);

	printf("Serializing dict\n");
	unsigned long size = 0;
	void * dictBuffer = dict->Serialize(&size);
	printf("\tSize: %d\n", size);

	printf("Deserializing buffer\n");
	BlockDictionary * dict2 = BlockDictionary::Deserialize(dictBuffer);

	printf("Test complete.\n");

	dict->ToFile("blocks.dict");
}